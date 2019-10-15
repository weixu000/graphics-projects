#version 330 core

in vec3 vNormal, vPos;

//uniform vec3 ks, kd, ka;
//uniform float alpha;
//uniform vec3 lightPos, lightColor, ambientColor;
//uniform vec3 viewPos;

out vec4 fragColor;

float cosP(vec3 x, vec3 y){
    return max(0.0f, dot(x, y));
}

void main()
{
    vec3 ks = vec3(0.0f), kd = vec3(1.0f), ka = vec3(0.0f);
    float alpha = 10.0f;
    vec3 lightPos = vec3(50.0f), lightColor = vec3(1.0f), ambientColor = vec3(1.0f);
    vec3 viewPos = vec3(0.0f, 0.0f, 20.0f);

    vec3 L = normalize(lightPos - vPos);
    vec3 N = normalize(vNormal);
    vec3 V = normalize(viewPos - vPos);
    vec3 R = 2 * dot(L, N) * N - L;
    fragColor = vec4(ka * ambientColor
    + kd * lightColor * cosP(L, N)
    + ks * lightColor * pow(cosP(R, V), alpha),
    1.0f);
}