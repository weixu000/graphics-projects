#version 330 core

in vec3 vNormal, vPos;

uniform vec3 ks, kd, ka;
uniform float alpha;

uniform vec3 lightPos, lightColor;
uniform float attenuation;

uniform vec3 viewPos;

out vec4 fragColor;

float cosP(vec3 x, vec3 y){
    return max(0.0f, dot(x, y));
}

void main()
{
    vec3 L = normalize(lightPos - vPos);
    vec3 N = normalize(vNormal);
    vec3 V = normalize(viewPos - vPos);
    vec3 R = 2 * dot(L, N) * N - L;

    float d = length(lightPos - vPos);
    vec3 lc = lightColor / (attenuation * d);

    fragColor = vec4(ka * lc
    + kd * lc * cosP(L, N)
    + ks * lc * pow(cosP(R, V), alpha),
    1.0f);
}