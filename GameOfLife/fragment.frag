#version 330 core

in vec2 Tex;

out vec4 FragColor;

uniform sampler2DArray gof_packed_texture;
uniform int StartingIndex;

void main()
{
    float index = 0;
    for(int i = StartingIndex+7; i > StartingIndex; i--, index++)
    { 
        if(texture(gof_packed_texture, vec3(Tex, i%8)).r > 0.0)
        {
            break;
        }
    }

    index = pow(2, -index);

    FragColor = vec4(index,index,index,1.0);
} 