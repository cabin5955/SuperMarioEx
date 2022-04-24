//
//  post_processor.cpp
//  LearnOpenGL_BreakOut
//
//  Created by chen caibin on 2021/9/26.
//

#include "post_processor.hpp"
#include <iostream>

PostProcessor::PostProcessor(Shader shader, GLuint width, GLuint height)
    : PostProcessingShader(shader), Texture(), Width(width), Height(height), Confuse(GL_FALSE), Chaos(GL_FALSE), Shake(GL_FALSE)
{
    glGetIntegerv(GL_FRAMEBUFFER_BINDING, &this->DefaultFBO);
    this->BindFramebufferTexture2D(width, height);
    
    // Initialize render data and uniforms
    this->initRenderData();
    this->PostProcessingShader.use();
    this->PostProcessingShader.setInt("scene", 0);
    GLfloat offset = 1.0f / 300.0f;
    GLfloat offsets[9][2] = {
        { -offset,  offset  },  // top-left
        {  0.0f,    offset  },  // top-center
        {  offset,  offset  },  // top-right
        { -offset,  0.0f    },  // center-left
        {  0.0f,    0.0f    },  // center-center
        {  offset,  0.0f    },  // center - right
        { -offset, -offset  },  // bottom-left
        {  0.0f,   -offset  },  // bottom-center
        {  offset, -offset  }   // bottom-right
    };
    glUniform2fv(glGetUniformLocation(this->PostProcessingShader.ID, "offsets"), 9, (GLfloat*)offsets);
    GLint edge_kernel[9] = {
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
    };
    glUniform1iv(glGetUniformLocation(this->PostProcessingShader.ID, "edge_kernel"), 9, edge_kernel);
    GLfloat blur_kernel[9] = {
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16
    };
    glUniform1fv(glGetUniformLocation(this->PostProcessingShader.ID, "blur_kernel"), 9, blur_kernel);
    
}

void PostProcessor::BindFramebufferTexture2D(int width,int height)
{
    this->bindMode = BIND_TEXTURE2D;
    glBindTexture(GL_TEXTURE_2D, this->Texture.ID);
    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width/2, height/2, 0, GL_RGBA,GL_UNSIGNED_BYTE, NULL);
    glBindTexture(GL_TEXTURE_2D, 0);

    glGenFramebuffers(1, &this->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER,this->FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.ID, 0);

    GLint errNo = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if(GL_FRAMEBUFFER_COMPLETE != errNo){
        printf("Unable to create FBO. errNo: %x\n",errNo);
    }
    glBindFramebuffer(GL_FRAMEBUFFER, this->DefaultFBO);
}

void PostProcessor::BindFramebufferNormal(int width,int height)
{
    this->bindMode = BIND_NORMAL;
    //framebuffer configuration
    glGenFramebuffers(1,&this->FBO);
    glBindFramebuffer(GL_FRAMEBUFFER,this->FBO);
    
    //create a color attachment texture
    this->Texture.Generate(width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,this->Texture.ID,0);
    
    // create a renderbuffer object for depth and stencil attachment
    glGenRenderbuffers(1,&this->RBO);
    glBindRenderbuffer(GL_RENDERBUFFER,this->RBO);
    // use a single renderbuffer object for both a depth AND stencil buffer.
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
    // now actually attach it
    // now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, this->RBO);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
    
    glBindFramebuffer(GL_FRAMEBUFFER, this->DefaultFBO);
}

void PostProcessor::BindFramebufferMultisample(int width,int height)
{
    this->bindMode = BIND_MULTISAMPLE;
    // Initialize renderbuffer/framebuffer object
    glGenFramebuffers(1, &this->MSFBO);
    glGenFramebuffers(1, &this->FBO);
    glGenRenderbuffers(1, &this->RBO);
    
    // Initialize renderbuffer storage with a multisampled color buffer (don't need a depth/stencil buffer)
    glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
    glBindRenderbuffer(GL_RENDERBUFFER, this->RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_RGB8, width, height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, this->RBO);
    
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize MSFBO" << std::endl;
        
    // Also initialize the FBO/texture to blit multisampled color-buffer to; used for shader operations (for postprocessing effects)
    glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
    this->Texture.Generate(width, height, NULL);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, this->Texture.ID, 0); // Attach texture to framebuffer as its color attachment
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "ERROR::POSTPROCESSOR: Failed to initialize FBO" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, this->DefaultFBO);
}

void PostProcessor::BeginRender()
{
    if(this->bindMode == BIND_MULTISAMPLE)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->MSFBO);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
    else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->FBO);
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}

void PostProcessor::EndRender()
{
    if(this->bindMode == BIND_MULTISAMPLE)
    {
        // Now resolve multisampled color-buffer into intermediate FBO to store to texture
        glBindFramebuffer(GL_READ_FRAMEBUFFER, this->MSFBO);
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->FBO);
        glBlitFramebuffer(0, 0, this->Width, this->Height, 0, 0, this->Width, this->Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
        glBindFramebuffer(GL_FRAMEBUFFER, this->DefaultFBO); // Binds both READ and WRITE framebuffer to default framebuffer
    }
    else
    {
        // now bind back to default framebuffer and draw a quad plane with the attached framebuffer color texture
        glBindFramebuffer(GL_FRAMEBUFFER, this->DefaultFBO);
        glDisable(GL_DEPTH_TEST);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}

void PostProcessor::Render(GLfloat time)
{
    // Set uniforms/options
    this->PostProcessingShader.use();
    this->PostProcessingShader.setFloat("time", time);
    this->PostProcessingShader.setInt("confuse", this->Confuse);
    this->PostProcessingShader.setInt("chaos", this->Chaos);
    this->PostProcessingShader.setInt("shake", this->Shake);
    // Render textured quad
    glActiveTexture(GL_TEXTURE0);
    this->Texture.Bind();
    glBindVertexArray(this->VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void PostProcessor::initRenderData()
{
    // Configure VAO/VBO
    GLuint VBO;
    GLfloat vertices[] = {
        // Pos        // Tex
        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f,
        -1.0f,  1.0f, 0.0f, 1.0f,

        -1.0f, -1.0f, 0.0f, 0.0f,
         1.0f, -1.0f, 1.0f, 0.0f,
         1.0f,  1.0f, 1.0f, 1.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(this->VAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GL_FLOAT), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
