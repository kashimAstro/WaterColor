varying vec2 tc;
varying vec4 color;

void main() {
        color = gl_Color;
        tc = gl_MultiTexCoord0.st;
        gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
