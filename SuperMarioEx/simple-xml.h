/*
    TODO List:
    
    - Handle comments
    - Handle CDATA tags
    - Handle allocations larger than XML_ALLOCATOR_BLOCK_SIZE
        (Initialize a new XmlAllocatorBlock that is big enough to handle
         the large allocation)
    
    Finalization:
    - Make methods static or "internal" that do not need to be exposed
    - Add some comments
    - Add some instructions
    - Tidy the code
    - Make sure no memory leaks are present.
    - Search for TODO's and fix em
    - Custom allocator support.
    - Custom file loader support.
    - Removal of all standard library dependencies.
    
    Possible features:
    - Handle both text and elements within a single element
        (This likely means converting text into a type of node that can be
         stored in the tree. This allows multiple bits of text to be split
         by an element, e.g. "blah blah <image /> blah blah")
    - Handle unclosed elements (close them at the first close that is not them)
    - Actually read the XML header?
*/

/* 
 * originally from https://github.com/Jools64/Simple-Xml
 * under the MIT License
 * 
 * modifications by Chris Camacho (2021) (codifies -  https://bedroomcoders.co.uk/)
 * 
 * compiler warnings in structs fixed
 * added xmlNodeCountAllChildrenWithName
 * fixed unaligned memory blocks from allocator
 * added implementation guard - think I'm calling this a fork now!
 * 
 * exactly one source unit should define SIMPLE_XML_IMPLEMENTATION
 * and import this header
 * any other unit that needs these fuctions should just include it
 * without the define.
 */



#include "stdlib.h"
#include "stdio.h"
#include "string.h"

#define xmlClamp(value, xmlMin, max) value < xmlMin ? xmlMin : value > max ? max : value
#define xmlMin(a, b) a < b ? a : b
#define xmlAssert(expression, output) if(!(expression)){printf("ASSERTION FAILED: " output "\n\n");abort();}

//------------------------------------------------------------------------------
// HEADER
//------------------------------------------------------------------------------

#define TEXT_BUFFER_BLOCK_SIZE 1024 * 2

#define XML_ALLOCATOR_BLOCK_SIZE 1024 * 1024 // 1MB
//typedef struct XmlAllocatorBlock XmlAllocatorBlock;
typedef struct XmlAllocatorBlock 
{
    unsigned char* memory;
    int size, offset;
    struct XmlAllocatorBlock* next;
} XmlAllocatorBlock;



//typedef struct XmlAttribute XmlAttribute;
typedef struct XmlAttribute
{
    struct XmlAttribute* next;
    char* name;
    char* value;
} XmlAttribute;

//typedef struct XmlNode XmlNode;
typedef struct XmlNode 
{
    char* text;
    char* name;
    struct XmlNode* parent;
    struct XmlNode* firstChild;
    struct XmlNode* lastChild;
    struct XmlNode* next;
    XmlAttribute* firstAttribute;
    XmlAttribute* lastAttribute;
} XmlNode;



#define XML_DOCUMENT_BUFFER_SIZE 1024 * 16 // This must be less or equal to the allocator size
typedef struct XmlDocument 
{
    FILE* file;
    long length, documentPosition;
    int bufferPosition, bufferContentLength;
    char* buffer;
    char readAhead[4];
    unsigned char readAheadInitialized;
    int readAheadPosition;
    unsigned char firstRead;
    XmlAllocatorBlock allocator;
    XmlNode* root;
} XmlDocument;

//typedef struct XmlStringBuffer XmlStringBuffer;
typedef struct XmlStringBuffer
{
    char* data;
    int length, size;
    struct XmlStringBuffer* next;
    XmlAllocatorBlock* allocator;
    unsigned char active; // Used when a buffer with multiple allocations has been cleared.
    unsigned char padding; // alignment 
} XmlStringBuffer;



/*

    Usage functions:
*/
void xmlDocumentLoad(XmlDocument* d, const char* filePath);
XmlNode* xmlNodeFindChildWithName(XmlNode* n, char* name);
int xmlNodeGetAllChildrenWithName(XmlNode* node, char* name, XmlNode** destinationArray, int max);
XmlNode* xmlNodeQuery(XmlNode* node, char* path, XmlDocument* d);
XmlNode* xmlDocumentQuery(XmlDocument* d, char* path);
char* xmlNodeGetAttribute(XmlNode* n, char* name);
void xmlDocumentFree(XmlDocument* d);


#ifdef SIMPLE_XML_IMPLEMENTATION

//------------------------------------------------------------------------------
// IMPLEMENTATION
//------------------------------------------------------------------------------

void xmlAllocatorInit(XmlAllocatorBlock* allocator);
void xmlAllocatorFree(XmlAllocatorBlock* a);
void* xmlAllocatorAllocate(XmlAllocatorBlock* a, int size);

void xmlNodeAddChild(XmlNode* parent, XmlNode* child);
void xmlStringBufferInit(XmlStringBuffer* b, int size, XmlAllocatorBlock* a);
void xmlStringBufferAppend(XmlStringBuffer* b, char* string, int count);
char* xmlStringBufferToString(XmlStringBuffer* b, XmlAllocatorBlock* a);
void xmlStringBufferClear(XmlStringBuffer* b);
char* xmlStringClone(char* s, XmlAllocatorBlock* a);

void xmlAllocatorInit(XmlAllocatorBlock* allocator)
{
    XmlAllocatorBlock a;
    
    // TODO: graceful error if malloc fails
    a.memory = (unsigned char*)malloc(XML_ALLOCATOR_BLOCK_SIZE);
    a.size = XML_ALLOCATOR_BLOCK_SIZE;
    a.offset = 0;
    a.next = 0;
    
    *allocator = a;
}

void xmlAllocatorFree(XmlAllocatorBlock* a)
{
    free(a->memory);
    if(a->next)
    {
        xmlAllocatorFree(a->next);
        free(a->next);
    }
}

void* xmlAllocatorAllocate(XmlAllocatorBlock* a, int size)
{
    if (size % 8 != 0) 
    {
        size += 8 - size % 8;
    }

    xmlAssert(size <= a->size, "Attempted to allocate an amount of memory that is \
greater than the size of single block");

    
    
    if(a->size - a->offset >= size)
    {
        void* memory = &a->memory[a->offset];
        a->offset += size;
        return memory;
    }
    else if(a->next)
    {
        return xmlAllocatorAllocate(a->next, size);
    }
    else
    {
        a->next = (XmlAllocatorBlock*) malloc(sizeof(XmlAllocatorBlock));
        xmlAllocatorInit(a->next);
        return xmlAllocatorAllocate(a->next, size);
    }
}

unsigned char xmlDocumentGetNextCharInternal(XmlDocument* d, char* dest)
{
    if(d->bufferContentLength > 0)
    {
        *dest = d->buffer[d->bufferPosition];
        ++d->bufferPosition;
        ++d->documentPosition;
        --d->bufferContentLength;
        return 1;
    }
    
    *dest = 0;
    
    if(d->documentPosition < d->length)
    {
        if(d->firstRead == 1 || d->bufferContentLength <= 0)
        {
            d->firstRead = 0;
            d->bufferContentLength = fread(d->buffer, 1, 
                                           xmlClamp((d->length - d->documentPosition), 
                                                 0, XML_DOCUMENT_BUFFER_SIZE), 
                                           d->file);
            
            if(d->bufferContentLength == 0)
                return 0;
            
            // TODO: ensure moving this did not introduce any bugs
            //d->documentPosition += d->bufferPosition;
            d->bufferPosition = 0;
        }
        
        // TODO: Ensure that this can only recurse a single time.
        return xmlDocumentGetNextCharInternal(d, dest);
    }
    
    return 0;
}

unsigned char xmlDocumentGetNextChar(XmlDocument* d, char* dest)
{
    int i;
    
    if(d->readAheadInitialized)
    {
        i = (d->readAheadPosition + 3) % 4;
        if(!xmlDocumentGetNextCharInternal(d, &d->readAhead[i]))
            d->readAhead[i] = -1;//255;
    }
    else
    {
        for(i = 0; i < 4; ++i)
        {
            if(!xmlDocumentGetNextCharInternal(d, &d->readAhead[i]))
                d->readAhead[i] = -1;//255;
        }
        d->readAheadInitialized = 1;
        d->readAheadPosition = 0;
    }
    
    unsigned char c = d->readAhead[d->readAheadPosition];
    d->readAheadPosition = ((d->readAheadPosition + 1) % 4);
    
    if(c == 255)
        return 0;
    else
        *dest = c;
    return 1;
}

unsigned char xmlCharacterIsWhiteSpace(char c)
{
    return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

unsigned char xmlCharacterIsIdentifierLegal(char c)
{
    return (c >= 'a' && c <= 'z') 
        || (c >= 'A' && c <= 'Z') 
        || (c >= '0' && c <= '9')
        || c == '_' || c == '-' || c == '.' || c == ':';
}

void xmlScanError(XmlDocument* d, char* error)
{
    // TODO: Keep track of line and column for this error reporting
    printf("Scan error at character %ld : %s\n", d->documentPosition, error);
}

typedef enum XmlScanState
{
    XmlParseState_none,
    XmlParseState_tag,
    XmlParseState_tagName,
    XmlParseState_tagSelfClose,
    XmlParseState_content,
    XmlParseState_text,
    XmlParseState_attributes,
    XmlParseState_attributeName,
    XmlParseState_attributeValue,
    XmlParseState_header,
} XmlParseState;

void xmlNodeAddChild(XmlNode* parent, XmlNode* child)
{
    if(parent->lastChild)
        parent->lastChild->next = child;
    else
        parent->firstChild = child;
    
    parent->lastChild = child;
    child->parent = parent;
}

void xmlNodeAddAttribute(XmlNode* n, XmlAttribute* a)
{
    if(n->lastAttribute)
        n->lastAttribute->next = a;
    else
        n->firstAttribute = a;
    
    n->lastAttribute = a;
}

void xmlNodePrintToConsole(XmlNode* node, int depth)
{
    int i;
    for(i = 0; i < depth * 2; ++i)
        printf("-");
    printf("%s", node->name);
    
    if(node->firstAttribute)
    {
        printf("(");
        
        XmlAttribute* a;
        for(a = node->firstAttribute;
            a != 0;
            a = a->next)
        {
            printf("%s=\"%s\"", a->name, a->value);
            if(a->next)
                printf(", ");
        }
        
        printf(")");
    }
    
    if(node->text)
        printf(": \"%s\"", node->text);
    printf("\n");
    
    if(node->firstChild)
    {
        XmlNode* childNode;
        for(childNode = node->firstChild;
            childNode != 0;
            childNode = childNode->next)
            xmlNodePrintToConsole(childNode, depth + 1);
    }
}

XmlNode* xmlDocumentCreateNode(XmlDocument* d, char* name)
{
    XmlNode* node = (XmlNode*)xmlAllocatorAllocate(&d->allocator, sizeof(XmlNode));
    memset(node, 0, sizeof(XmlNode));
    node->name = xmlStringClone(name, &d->allocator);
    return node;
}

XmlAttribute* xmlDocumentCreateAttribute(XmlDocument* d, char* name)
{
    XmlAttribute* attribute = (XmlAttribute*)xmlAllocatorAllocate(&d->allocator, sizeof(XmlNode));
    memset(attribute, 0, sizeof(XmlAttribute));
    attribute->name = xmlStringClone(name, &d->allocator);
    return attribute;
}

void xmlAttributeSetValue(XmlDocument* d, XmlAttribute* a, char* value)
{
    a->value = xmlStringClone(value, &d->allocator);
}

void xmlScanDocument(XmlDocument* d)
{
    XmlParseState state;
    int elementDepth;
    unsigned char closingTag, doubleQuotes;
    XmlStringBuffer buffer;
    XmlNode* currentNode;
    XmlAttribute* currentAttribute;
    char c;
    
    elementDepth = 0;
    closingTag = 0;
    doubleQuotes = 1;
    state = XmlParseState_none;
    xmlStringBufferInit(&buffer, TEXT_BUFFER_BLOCK_SIZE, &d->allocator);
    currentNode = xmlDocumentCreateNode(d, "root");
    d->root = currentNode;
    
    while(xmlDocumentGetNextChar(d, &c))
    {
        //printf("%c", c);
        
        switch(state)
        {
            case XmlParseState_none:
                while(xmlCharacterIsWhiteSpace(c))
                    if(!xmlDocumentGetNextChar(d, &c))
                        continue;
                if(c == '<')
                {
                    state = XmlParseState_tag;
                }
                else
                {
                    printf("C: \"%d\"" , c);
                    xmlScanError(d, "expected tag");
                }
                break;
                
            case XmlParseState_tag:
                if(c == '?')
                {
                    state = XmlParseState_header;
                }
                else if(c == '/')
                {
                    state = XmlParseState_tagName;
                    closingTag = 1;
                }
                else if((c >= 'a' && c <= 'z') 
                || (c >= 'A' && c <= 'Z') || c == '_')
                {
                    xmlStringBufferAppend(&buffer, &c, 1);
                    state = XmlParseState_tagName;
                    closingTag = 0;
                }           
                break;
                
            case XmlParseState_header:
                if(c == '?')
                {
                    if(!xmlDocumentGetNextChar(d, &c))
                    {
                        xmlScanError(d, "Header never closed");
                    }
                    else if(c == '>')
                        state = XmlParseState_none;
                }
                
                break;
                
            case XmlParseState_tagName:
                if(xmlCharacterIsIdentifierLegal(c))
                    xmlStringBufferAppend(&buffer, &c, 1);
                else
                {
                    char* tagName = xmlStringBufferToString(&buffer, &d->allocator);
                    xmlStringBufferClear(&buffer);
                    if(!closingTag)
                    {
                        elementDepth++;
                        //printf("TAG: %s\n", tagName);
                        
                        XmlNode* node = xmlDocumentCreateNode(d, tagName);
                        xmlNodeAddChild(currentNode, node);
                        currentNode = node;
                    }
                    else
                    {
                        elementDepth--;
                        //printf("CLOSING TAG: %s\n", tagName);
                        currentNode = currentNode->parent;
                        // TODO: Verify that the tag being closed is infact
                        //       the current node so that errors can be 
                        //       reported.
                    }
                    
                    while(xmlCharacterIsWhiteSpace(c))
                        if(!xmlDocumentGetNextChar(d, &c))
                            continue;
                    
                    if(c == ':' || c == '=' 
                    || (c >= 'a' && c <= 'z') 
                    || (c>= 'A' && c <= 'Z'))
                    {
                        xmlStringBufferAppend(&buffer, &c, 1);
                        state = XmlParseState_attributeName;
                    }
                    else if(c == '>')
                        state = XmlParseState_content;
                    else if(c == '/')
                        state = XmlParseState_tagSelfClose;
                }
                break;
                
            case XmlParseState_tagSelfClose:
                if(c == '>')
                {
                    elementDepth--;
                    currentNode = currentNode->parent;
                    
                    if(elementDepth > 0)
                        state = XmlParseState_content;
                    else 
                        state = XmlParseState_none;
                }
                else
                    xmlScanError(d, "expected >");
                break;
                
            case XmlParseState_attributeName:
                if(buffer.length == 0)
                {
                    while(xmlCharacterIsWhiteSpace(c))
                        if(!xmlDocumentGetNextChar(d, &c))
                            continue;
                    if(c == '>')
                        state = XmlParseState_content;
                    else if(c == '/')
                        state = XmlParseState_tagSelfClose;
                    else if(xmlCharacterIsIdentifierLegal(c))
                        xmlStringBufferAppend(&buffer, &c, 1);
                    else
                        xmlScanError(d, "unexpected symbol");
                }
                else
                {
                    if(xmlCharacterIsIdentifierLegal(c))
                    {
                        xmlStringBufferAppend(&buffer, &c, 1);
                    }
                    else
                    {
                        if(xmlCharacterIsWhiteSpace(c))
                        {
                            while(xmlCharacterIsWhiteSpace(c))
                                if(!xmlDocumentGetNextChar(d, &c))
                                    continue;
                        }
                        if(c == '=')
                        {
                            if(!xmlDocumentGetNextChar(d, &c))
                                continue;
                            while(xmlCharacterIsWhiteSpace(c))
                                if(!xmlDocumentGetNextChar(d, &c))
                                    continue;
                            if(c == '"' || c == '\'')
                            {
                                doubleQuotes = c == '"';
                                
                                char* attributeName = xmlStringBufferToString(&buffer, &d->allocator);
                                xmlStringBufferClear(&buffer);
                                //printf("ATTRIBUTE: %s\n", attributeName);
                                
                                currentAttribute = xmlDocumentCreateAttribute(d, attributeName);
                                xmlNodeAddAttribute(currentNode, currentAttribute);
                                
                                state = XmlParseState_attributeValue;
                            }
                            else
                                xmlScanError(d, "expected attribute value");
                        }
                        else
                            xmlScanError(d, "illegal character in attribute name");
                    }
                }
                break;
                
            case XmlParseState_attributeValue:
                if((doubleQuotes && c == '"') || (!doubleQuotes && c == '\''))
                {
                    char* attributeValue = xmlStringBufferToString(&buffer, &d->allocator);
                    xmlStringBufferClear(&buffer);
                    //printf("ATTRIBUTE VALUE: %s\n", attributeValue);
                    
                    xmlAttributeSetValue(d, currentAttribute, attributeValue);
                    
                    state = XmlParseState_attributeName;
                }
                else
                {
                    // TODO: Ensure string only contains valid characters
                    if(c == '\\')
                    {
                        if(!xmlDocumentGetNextChar(d, &c))
                            continue;
                    }
                    xmlStringBufferAppend(&buffer, &c, 1);
                }
                break;
                    
            case XmlParseState_content:
                if(xmlCharacterIsWhiteSpace(c))
                    continue;
                else if(c == '<')
                {
                    state = XmlParseState_tag;
                    elementDepth++;
                }
                else
                {
                    state = XmlParseState_text;
                    xmlStringBufferAppend(&buffer, &c, 1);
                }
                break;
            
            case XmlParseState_text:
                if(c == '<')
                {
                    char* text = xmlStringBufferToString(&buffer, &d->allocator);
                    xmlStringBufferClear(&buffer);
                    //printf("TEXT: %s\n", text);
                    
                    currentNode->text = xmlStringClone(text, &d->allocator);
                    
                    state = XmlParseState_tag;
                }
                else
                {
                    xmlStringBufferAppend(&buffer, &c, 1);
                }
                break;
            
            default:
                printf("Warning: Unhandled state\n");
                break;
        }
    }
}

void xmlDocumentLoad(XmlDocument* d, const char* filePath)
{
    d->firstRead = 1;
    d->bufferPosition = 0;
    d->bufferContentLength = 0;
    d->documentPosition = 0;
    d->length = 0;
    d->readAheadInitialized = 0;
    
    d->readAheadPosition = 0;
    memset(&d->readAhead, 0, 4);
    
    // Load the document and find it's size
    d->file = fopen(filePath, "r");
    fseek(d->file, 0, SEEK_END);
    d->length = ftell(d->file);
    fseek(d->file, 0, SEEK_SET);
    
    //printf("Reading file with length of %ld\n\n", d->length);
    
    // Initialize a memory allocator and get buffer
    xmlAllocatorInit(&d->allocator);
    d->buffer = (char*)xmlAllocatorAllocate(&d->allocator, XML_DOCUMENT_BUFFER_SIZE);
    d->documentPosition = 0;
    
    
    xmlScanDocument(d);
    
    //printf("\n\n");
    
    //xmlNodePrintToConsole(d->root, 0);
    
    //printf("\n\n");
    
    fclose(d->file);
}

XmlNode* xmlNodeFindChildWithName(XmlNode* n, char* name)
{
    XmlNode* node = n->firstChild;
    
    while(node)
    {
        if(strcmp(node->name, name) == 0)
            return node;
        node = node->next;
    }
    
    return 0;
}

int xmlNodeGetAllChildrenWithName(XmlNode* node, char* name, XmlNode** destinationArray, int max)
{
    int count = 0;
    
    node = node->firstChild;
    
    while(node)
    {
        if(strcmp(node->name, name) == 0)
            destinationArray[count++] = node;
        
        if(count == max)
            break;
        
        node = node->next;
    }
    return count;
}

int xmlNodeCountAllChildrenWithName(XmlNode* node, char* name)
{
    int count = 0;
    
    node = node->firstChild;
    
    while(node)
    {
        if(strcmp(node->name, name) == 0) count++;
        node = node->next;
    }
    return count;
}

XmlNode* xmlNodeQuery(XmlNode* node, char* path, XmlDocument* d)
{
    int pathLength = strlen(path);
    char* buffer = (char*)xmlAllocatorAllocate(&d->allocator, pathLength);
    int bufferPosition = 0;
    int i;
    
    for(i = 0; i < pathLength; ++i)
    {
        if(path[i] != '.')
        {
            buffer[bufferPosition++] = path[i];
        }
        
        if(path[i] == '.' || i == pathLength - 1)
        {
            buffer[bufferPosition] = '\0';
            bufferPosition = 0;
            node = xmlNodeFindChildWithName(node, buffer);
            //if(node)
            //   printf("Found the node\n");
        }
        
        if(node == 0)
            break;
    }
    
    return node;
}

XmlNode* xmlDocumentQuery(XmlDocument* d, char* path)
{
    return xmlNodeQuery(d->root, path, d);
}

char* xmlNodeGetText(XmlNode* n)
{
    return n->text;
}

char* xmlNodeGetAttribute(XmlNode* n, char* name)
{
    XmlAttribute* attribute = n->firstAttribute;
    
    while(attribute)
    {
        if(strcmp(attribute->name, name) == 0)
            return attribute->value;
        attribute = attribute->next;
    }
    
    return 0;
}

void xmlDocumentFree(XmlDocument* d)
{
    xmlAllocatorFree(&d->allocator);
    // TODO: Free all Nodes here
}

void xmlStringBufferInit(XmlStringBuffer* b, int size, XmlAllocatorBlock* a)
{
    b->data = (char*) xmlAllocatorAllocate(a, size);
    b->size = size;
    b->length = 0;
    b->next = 0;
    b->allocator = a;
    b->active = 1;
    xmlStringBufferClear(b);
}

void xmlStringBufferAppend(XmlStringBuffer* b, char* string, int count)
{
    if(b->next && b->next->active) // This buffer is full so move to the next in chain
    {
        xmlStringBufferAppend(b->next, string, count);
    }
    else if(count + b->length < b->size)
    {
        b->length += count;
        strncat(b->data, string, xmlMin(count, b->size - b->length));
    }
    else
    {
        // Jump to another buffer.
        if(b->next == 0)
        {
            b->next = (XmlStringBuffer*)xmlAllocatorAllocate(b->allocator, sizeof(XmlStringBuffer));
            xmlStringBufferInit(b->next, b->size, b->allocator) ;
        }
        else
            b->next->active = 1;
        
        xmlStringBufferAppend(b->next, string, count);
    }
}

char* xmlStringBufferToString(XmlStringBuffer* b, XmlAllocatorBlock* a)
{
    // TODO: Should this be strlen(b->data)+1 or just strlen(b->data)?
    
    int length = 0;
    XmlStringBuffer* i;
    for(i = b; i != 0; i = i->next)
        length += i->length;
    
    char* string = (char*)xmlAllocatorAllocate(a, length+1);
    strcpy(string, b->data);
    for(i = b->next; i != 0 && i->active; i = i->next)
        strcat(string, i->data);
    return string;
}

char* xmlStringClone(char* s, XmlAllocatorBlock* a)
{
    char* string = (char*)xmlAllocatorAllocate(a, strlen(s) + 1);
    strcpy(string, s);
    
    return string;
}

void xmlStringBufferClear(XmlStringBuffer* b)
{
    b->length = 0;
    b->data[0] = 0;
    
    XmlStringBuffer* i;
    for(i = b->next; i != 0 && i->active; i = i->next)
    {
        i->active = 0;
        i->length = 0;
        i->data[0] = 0;
    }
}
#endif // include guard

