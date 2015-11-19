varying vec2 tc;
varying vec4 color;

uniform sampler2D colorMap;
uniform sampler2D heightMap;

uniform float gradientStep;
uniform float advectStep;
uniform float flipHeightMap;
uniform float time;
uniform vec4 advectMatrix;

void main() {

     vec4 cxp = texture2D(heightMap, vec2(tc.x + gradientStep, flipHeightMap*tc.y));
     vec4 cxn = texture2D(heightMap, vec2(tc.x - gradientStep, flipHeightMap*tc.y));
     vec4 cyp = texture2D(heightMap, vec2(tc.x, flipHeightMap*(tc.y + gradientStep)));
     vec4 cyn = texture2D(heightMap, vec2(tc.x, flipHeightMap*(tc.y - gradientStep)));

     vec3 grey = vec3(.3, .59, .11);
     float axp = dot(grey, cxp.xyz);
     float axn = dot(grey, cxn.xyz);
     float ayp = dot(grey, cyp.xyz);
     float ayn = dot(grey, cyn.xyz);

     vec2 grad = vec2(axp - axn, ayp - ayn);
     vec2 newtc = tc + advectStep * normalize(advectMatrix.xy * grad) * time;

     gl_FragColor.rgb = texture2D(colorMap, newtc).rgb * color.rgb;
     gl_FragColor.a = color.a;
     //gl_FragColor = cyn;//texture2D(colorMap, tc);
}

