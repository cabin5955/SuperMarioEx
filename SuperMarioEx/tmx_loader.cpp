//
//  tmx_loader.cpp
//  SuperMarioEx
//
//  Created by chen caibin on 2022/3/26.
//

#define SIMPLE_XML_IMPLEMENTATION
#include "tmx_loader.hpp"
#include "resource_manager.hpp"
#include "simple-xml.h"
#include "global.hpp"

SpriteSheet* TmxLoader::createSpriteSheet(const char* textureName,
                                          glm::vec2 frameSize,
                                          int framesWide, glm::vec2 origin)
{
    SpriteSheet* sprite = (SpriteSheet*)malloc(sizeof(SpriteSheet));
    sprite->texture = ResourceManager::GetTexture(textureName);
    sprite->frameSize = frameSize;
    sprite->framesWide = framesWide;
    sprite->origin = origin;
    sprite->colShapes = 0;
    return sprite;
}

// free the resources used by a sprite sheet
void TmxLoader::freeSpriteSheet(SpriteSheet* ss)
{
    if (ss->colShapes) {
        for (int i=0; i<ss->tileCount; i++) {
            int np = ss->colShapes[i].numPolys;
            for (int j=0; j<np; j++) {
                Poly p = ss->colShapes[i].polys[j];
                free(p.points);
            }
            free(ss->colShapes[i].polys);
        }
        free(ss->colShapes);
    }
    //UnloadTexture(ss->texture);
    free(ss);
}

Tmx* TmxLoader::LoadTmx(const char* filename)
{
    const char* path;
    const char* file;
    
    char des[1024]={0};
    const char* tmp = Global::ResFullPath(des, filename);//GetDirectoryPath(filename); // static string !
    int tSz = strlen(tmp);
    path = (char*)malloc( tSz );
    memcpy((void*)path, (void*)tmp, tSz);

    //file = GetFileName(filename);  // just a pointer inside the original string no need to free here

    // important to clear out the space for the map
    // relying on zero defaults for some variables...
    Tmx* tmx = (Tmx*)calloc(1,sizeof(Tmx));
     
    // change to the data directory and load
    // the map and its dependancies
    XmlDocument document;
    
    //ChangeDirectory(path);
    
    xmlDocumentLoad(&document, des);
    
    free((void*)path);
    
    // map is the root node
    XmlNode* map = xmlDocumentQuery(&document, "map");
    
    // global map values
    tmx->mapWidth = atoi(xmlNodeGetAttribute(map, "width"));
    tmx->mapHeight = atoi(xmlNodeGetAttribute(map, "height"));
    tmx->tileWidth = atoi(xmlNodeGetAttribute(map, "tilewidth"));
    tmx->tileHeight = atoi(xmlNodeGetAttribute(map, "tileheight"));
    
    // get upto a maximum of 8 tilesets for the map (TODO look up number of nodes! remove max)
    XmlNode* tilesets[8];
    tmx->numTsets = xmlNodeGetAllChildrenWithName(map, "tileset", &tilesets[0], 8);
    
    // sorry I use spriteSheet and tileset term interchangably
    // sprite code was abstracted from a previous project.
    tmx->spriteSheets = (SpriteSheet**)malloc(sizeof(SpriteSheet*) * tmx->numTsets);

    // load each spritesheet from each of the tileset nodes
    for (int i = 0; i < tmx->numTsets; i++) {
        XmlDocument tsx;
        // each tileset has its own xml document
        char *fileName = xmlNodeGetAttribute(tilesets[i], "source");
        char temp[1024]={0};
        xmlDocumentLoad(&tsx,Global::ResFullPath(temp, fileName));
        XmlNode* ts = xmlDocumentQuery(&tsx, "tileset"); // root node
        
        int tw = atoi(xmlNodeGetAttribute(ts, "tilewidth"));
        int th = atoi(xmlNodeGetAttribute(ts, "tileheight"));
        int cols = atoi(xmlNodeGetAttribute(ts, "columns"));
        
        // actually crate a spritesheet for the tileset
        XmlNode* tsi = xmlDocumentQuery(&tsx, "tileset.image");
        tmx->spriteSheets[i] = createSpriteSheet(xmlNodeGetAttribute(tsi, "source"),
                                (glm::vec2){tw,th}, cols, (glm::vec2){tw/2.0, th});
        tmx->spriteSheets[i]->firstGid = atoi(xmlNodeGetAttribute(tilesets[i], "firstgid"));
        tmx->spriteSheets[i]->tileCount = atoi(xmlNodeGetAttribute(ts, "tilecount"));

        // get all the tile nodes
        int tn = xmlNodeCountAllChildrenWithName(ts, "tile");
        
        XmlNode** tileNodes = (XmlNode**)malloc(tn * sizeof(XmlNode*));
        xmlNodeGetAllChildrenWithName(ts,"tile",tileNodes,tn);
        
        // grab any collision shapes
        // TODO only polys supported
        // TODO should only probably allocate this if there are any collision shapes!
        tmx->spriteSheets[i]->colShapes = (PolyArrays*)calloc(tmx->spriteSheets[i]->tileCount, sizeof(PolyArrays));
        for (int j=0; j < tn; j++) {
            int tid = atoi(xmlNodeGetAttribute(tileNodes[j], "id"));
            XmlNode* og = xmlNodeFindChildWithName(tileNodes[j], "objectgroup");
            if(og == NULL) continue;;
            int no = xmlNodeCountAllChildrenWithName(og, "object");
            XmlNode** objNodes = (XmlNode**)malloc(no * sizeof(XmlNode*));
            xmlNodeGetAllChildrenWithName(og,"object",objNodes,no);
            
            tmx->spriteSheets[i]->colShapes[tid].polys = (Poly*)calloc(no, sizeof(Poly));
            for (int jj=0; jj<no; jj++) {
                // TODO only poly collision shape supported for now
                // TODO rotated shapes not supported
                XmlNode* pNode;
                pNode = xmlNodeFindChildWithName(objNodes[jj],"polygon");
                float offX = atoi(xmlNodeGetAttribute(objNodes[jj], "x"));
                float offY = atoi(xmlNodeGetAttribute(objNodes[jj], "y"));
                tmx->spriteSheets[i]->colShapes[tid].numPolys = no;
                char* ps = xmlNodeGetAttribute(pNode, "points");
                int psl = strlen(ps)+1;
                int nPnts = 0;
                for (int l=0; l<psl; l++) if (ps[l]==',') nPnts++;
                tmx->spriteSheets[i]->colShapes[tid].polys[jj].points = (glm::vec2*)malloc(sizeof(glm::vec2)*nPnts);
                tmx->spriteSheets[i]->colShapes[tid].polys[jj].numPoints=nPnts;
                char tmpStr[1024];
                unsigned int ts = 0;
                unsigned int te = 0;
                // parseout a string "x,y x,y x,y"
                // TODO verify last object gets its shape
                for (int l=0; l<nPnts; l++) {
                    while (ps[te]!=' ' && ps[te]!=0) te++;
                    memcpy(tmpStr,&ps[ts],te-ts);
                    tmpStr[te-ts]=0;
                    te++;
                    ts=te;
                    char* s2=tmpStr;
                    while(s2[0]!=',') s2++;
                    s2[0]=0;
                    s2++;
                    tmx->spriteSheets[i]->colShapes[tid].polys[jj].points[l] =
                                (glm::vec2){atof(tmpStr)+offX,atof(s2)+offY};
                }
            }
            free(objNodes);
        }
        xmlDocumentFree(&tsx);
        free(tileNodes);
    }
    

    // tile layers, the actual body of the map
    
    // TODO get actual number of nodes... remove maximum
    XmlNode* layerNodes[8];
    tmx->numLayers = xmlNodeGetAllChildrenWithName(map, "layer", layerNodes, 8);
    tmx->layers = (Layer**)malloc(sizeof(Layer*) * tmx->numLayers);
    for (int i=0; i < tmx->numLayers; i++) {
        tmx->layers[i] = (Layer*)malloc(sizeof(Layer));
    }
    
    // All layers are the same size as the map.
    for (int i=0; i < tmx->numLayers; i++) {
        XmlNode* data;
        data = xmlNodeFindChildWithName(layerNodes[i], "data");
        char* dstr = xmlNodeGetText(data);
        // the cells array is actually an array of columns
        // as I prefer to do array[x][y] rather than array[y][x]
        tmx->layers[i]->cells = (int**)malloc(sizeof(int*) * tmx->mapWidth);
        for (int j=0; j < tmx->mapWidth; j++) {
            tmx->layers[i]->cells[j] = (int*)malloc(sizeof(int) * tmx->mapHeight);
        }
        int pptr = 0;
        int sptr = 0;
        char tmp[16];
        // walk through other tokens assuming data string size is correct!
        // extra code by the strtok disapreciation society
        // relying on it being machine generated :-o
        int x = 0, y = 0;
        do {
            if (dstr[sptr]==',' || dstr[sptr]==0) {
                for (int j = pptr; j < sptr; j++) {
                    tmp[j-pptr] = dstr[j];
                }
                tmp[sptr-pptr]=0;
                tmx->layers[i]->cells[x][y] = atoi(tmp);
                x++;
                if (x > tmx->mapWidth-1) { x = 0; y += 1; }
                if (y > tmx->mapHeight-1) break;
                sptr++;
                while(dstr[sptr] < 32) { sptr++; }
                pptr = sptr;
            }
            sptr++;
        } while( true );  // ick, may well look at reimplementing this ?
        
    }
    
    // objects
    // TODO only single object group supported
    XmlNode* og = xmlNodeFindChildWithName(map, "objectgroup");
    TObject* objects = 0;
    if (og) {  // objects might not be in use...
        XmlNode** objNodes;
        tmx->numObj = xmlNodeCountAllChildrenWithName(og, "object");
        tmx->objects = (TObject*)malloc(sizeof(TObject*) * tmx->numObj);
        objNodes = (XmlNode**)malloc(sizeof(XmlNode*) * tmx->numObj);
        xmlNodeGetAllChildrenWithName(og, "object", objNodes, tmx->numObj);
            
        for (int i=0; i < tmx->numObj; i++) {
            tmx->objects[i].pos = (glm::vec2) {
                                    atof(xmlNodeGetAttribute(objNodes[i], "x")),
                                    atof(xmlNodeGetAttribute(objNodes[i], "y"))};
            tmx->objects[i].gid = atoi(xmlNodeGetAttribute(objNodes[i], "gid"));
            // TODO check map projection type
            // previously sorted for isometric...
            //objects[i].sorty = (objects[i].pos.x + objects[i].pos.y) / 2.0;
            tmx->objects[i].sorty = objects[i].pos.y;
        }
        free(objNodes);
    }
    
    // got all the data needed at this point all the xmlNode pointers
    // are pointing to released memory.
    xmlDocumentFree(&document);

    // Y sort objects
    // simple bubble sort is sufficient
    if (tmx->objects) {
    bool sort = true;
        while (sort) {
            sort = false;
            for (int i=0; i < tmx->numObj-1; i++) {
                if (tmx->objects[i].sorty > tmx->objects[i+1].sorty) {
                    TObject tmp = tmx->objects[i];
                    tmx->objects[i] = tmx->objects[i+1];
                    tmx->objects[i+1] = tmp;
                    sort = true;
                }
            }
        }
    }
    
    // phew we're done parsing!
    return tmx;
}

void TmxLoader::freeTmx(Tmx* tmx)
{
    for (int i = 0; i < tmx->numTsets; i++) {
        freeSpriteSheet(tmx->spriteSheets[i]);
    }
    free(tmx->spriteSheets);
    
    for(int i=0; i< tmx->numLayers; i++) {
        for (int j=0; j < tmx->mapWidth; j++) {
            free(tmx->layers[i]->cells[j]);
        }
        free(tmx->layers[i]->cells);
    }
    
    if (tmx->objects) free(tmx->objects);
}
