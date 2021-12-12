"use strict"

//--------------------------------------------------------------------------------------------------------
// VERTEX SHADER (GLSL language)
//--------------------------------------------------------------------------------------------------------

var sky_vert =
`#version 300 es

layout(location=0) in vec3 position_in;
out vec3 tex_coord;
uniform mat4 projectionviewMatrix;

void main()
{
 tex_coord = position_in;
 gl_Position = projectionviewMatrix * vec4(position_in,1.0);
}
`;

var sunVertexShader =
`#version 300 es

// INPUT
layout(location=1) in vec3 position_in;
layout(location=2) in vec2 textureCoord_in;

// UNIFORM
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

// OUPUT
out vec2 v_textureCoord;

// MAIN PROGRAM
void main()
{
	v_textureCoord = textureCoord_in;
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4( position_in, 1.0 );
}
`;

var vertexShader =
`#version 300 es

// INPUT
layout(location=1) in vec3 position_in;
layout(location=2) in vec2 textureCoord_in;
layout(location=3) in vec3 normal_in;

// UNIFORM
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

uniform mat3 uNormalMatrix;

// OUPUT
out vec2 v_textureCoord;
out vec3 v_position;
out vec3 v_normal;

// MAIN PROGRAM
void main()
{
    v_position = ( uViewMatrix * uModelMatrix * vec4( position_in, 1.0 ) ).xyz; 
    v_normal = normalize( uNormalMatrix * normal_in ); 
    
	v_textureCoord = textureCoord_in;
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4( position_in, 1.0 );
}
`;

var asteroidVertexShader = 
`#version 300 es
precision highp float;

// INPUT
layout(location=0) in vec3 position_in;
layout(location=2) in vec2 textureCoord_in;
layout(location=3) in mat4 instanceMatrix;

// UNIFORM
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uTimeMatrix;

// OUPUT
out vec2 v_textureCoord;

// MAIN PROGRAM
void main()
{
    v_textureCoord = textureCoord_in;

	gl_Position = uProjectionMatrix * uViewMatrix * uTimeMatrix * instanceMatrix * vec4( position_in, 1.0 );
}
`;

var GlowVertexShader = 
`#version 300 es
precision highp float;

// INPUT
layout(location=0) in vec3 position_in;

// UNIFORM
uniform mat4 uProjectionMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uModelMatrix;

// MAIN PROGRAM
void main()
{
	gl_Position = uProjectionMatrix * uViewMatrix * uModelMatrix * vec4( position_in, 1.0 );
}
`;

//--------------------------------------------------------------------------------------------------------
// FRAGMENT SHADER (GLSL language)
//--------------------------------------------------------------------------------------------------------
var sky_frag = 
`#version 300 es
precision highp float;
in vec3 tex_coord;
out vec4 frag;
uniform samplerCube TU;

void main()
{
 frag = texture(TU, tex_coord);
}
`;

var sunFragmentShader =
`#version 300 es
precision highp float;

// INPUT
in vec2 v_textureCoord;

// OUPUT
out vec4 oFragmentColor;

// UNIFORM
uniform sampler2D uSampler; 

// MAIN PROGRAM
void main()
{
    vec4 textureColor;
    textureColor = texture(uSampler, v_textureCoord );
	oFragmentColor = textureColor;
}
`;

var fragmentShader =
`#version 300 es
precision highp float;

// INPUT
in vec2 v_textureCoord;
in vec3 v_position;
in vec3 v_normal;

// OUPUT
out vec4 oFragmentColor;

// UNIFORM
uniform bool terre;
uniform sampler2D uSampler; 
uniform sampler2D uSampler2;
uniform sampler2D uSampler3;
uniform vec3 uLightPosition; 

// MAIN PROGRAM
void main()
{
    vec4 textureColor;
    vec4 nightTextureColor;
    if (terre)
    {
        textureColor = mix(texture(uSampler, v_textureCoord ),texture(uSampler2, v_textureCoord ),0.6);
        nightTextureColor = mix(texture(uSampler3, v_textureCoord ),texture(uSampler2, v_textureCoord ),0.1);
    }
    else
        textureColor = texture(uSampler, v_textureCoord );

    vec3 Ka = vec3(0); // ambiant
    vec3 Kd = vec3(textureColor); // diffuse
    vec3 Ks = vec3(0.2); // specular
    float Ns = 1.0; // specular
    vec3 Intensity = vec3(1);

    vec3 Ia = Intensity * Ka;
    vec3 lightdir = uLightPosition - v_position;
    vec3 Id = Intensity * Kd * dot(normalize(v_normal),normalize(lightdir));
    vec3 viewdir = vec3(0) - v_position;
    vec3 Is = Intensity * Ks * pow(dot(normalize(v_normal),normalize(lightdir+viewdir)),Ns);
    vec3 color = Ia + Id + Is;

    if (terre && color.x<0.1 && color.y<0.1 && color.z<0.1)
        oFragmentColor = nightTextureColor;
    else if (terre)
        oFragmentColor = textureColor;
    else 
        oFragmentColor = vec4(color,1);
}
`;

var AsteroidFragmentShader =
`#version 300 es
precision highp float;

// INPUT
in vec2 v_textureCoord;

// OUPUT
out vec4 oFragmentColor;

// UNIFORM
uniform sampler2D uSampler; 

// MAIN PROGRAM
void main()
{
    vec4 textureColor;
    textureColor = texture(uSampler, v_textureCoord );
	// MANDATORY
	oFragmentColor = textureColor;
}
`;

var GlowFragmentShader =
`#version 300 es
precision highp float;

// OUPUT
out vec4 oFragmentColor;

// UNIFORM
uniform vec3 uColor;

// MAIN PROGRAM
void main()
{
	oFragmentColor = vec4(uColor,1);
}
`;

// Global variables : textures, FBOs, prog_shaders, mesh, renderer, and a lot of parameters

var prg_skybox=null;
var sky_rend=null;
var tex_skybox=null;

var planetShaderProgram=null;
var sunShaderProgram=null;

var planet_rend=null;
var tex_earth=null;
var tex_earth2=null;
var tex_night_earth=null;
var tex_mars=null;
var tex_mercury=null;
var tex_venus=null;
var tex_jupiter=null;
var tex_saturn=null;
var tex_uranus=null;
var tex_neptune=null;

var sun_rend=null;
var tex_sun=null;

var tex_death_star=null;
var death_star_pos_x=130;
var death_star_pos_y=0;
var avance=1;
var death_star_speed=0.2;

var asteroidShaderProgram=null;
var asteroid_rend=null;
var asteroid_rend2=null;
var tex_asteroid=null;
var nbAsteroids=10000;
var nbAsteroids_saturn=3000;

var speed=1;
var scene_radius=150;
var scene_center=Vec3(0);

var glow_tex1=null;
var glow_tex2=null;
var fbo1=null;
var fbo2=null;
var glowShaderProgram=null;
var glow_rend=null;

//pour bouger le centre de la scene sur les differents obj
var focus=0; // 0 -> Sun
var mercury_pos=null; // focus = 1
var venus_pos=null; // focus = 2
var earth_pos=null; // focus = 3
var moon_pos=null; // focus = 4
var mars_pos=null; // focus = 5
var jupiter_pos=null; // focus = 6
var saturn_pos=null; // focus = 7
var uranus_pos=null; // focus = 8
var neptune_pos=null; // focus = 9


function init_wgl()
{
	ewgl.continuous_update = true;

    // Generate all the textures
    tex_earth = Texture2d();
    tex_earth.load("images/2k_earth_daymap.jpg",gl.RGB8);
    tex_earth2 = Texture2d();
    tex_earth2.load("images/2k_earth_clouds.jpg",gl.RGB8);
    tex_night_earth = Texture2d();
    tex_night_earth.load("images/2k_earth_nightmap.jpg",gl.RGB8);
    tex_mars = Texture2d();
    tex_mars.load("images/2k_mars.jpg",gl.RGB8);
    tex_mercury = Texture2d();
    tex_mercury.load("images/2k_mercury.jpg",gl.RGB8);
    tex_venus = Texture2d();
    tex_venus.load("images/2k_venus.jpg",gl.RGB8);
    tex_jupiter = Texture2d();
    tex_jupiter.load("images/2k_jupiter.jpg",gl.RGB8);
    tex_saturn = Texture2d();
    tex_saturn.load("images/2k_saturn.jpg",gl.RGB8);
    tex_uranus = Texture2d();
    tex_uranus.load("images/2k_uranus.jpg",gl.RGB8);
    tex_neptune = Texture2d();
    tex_neptune.load("images/2k_neptune.jpg",gl.RGB8);
    tex_sun = Texture2d();
    tex_sun.load("images/2k_sun.jpg",gl.RGB8);

    tex_death_star=Texture2d();
    tex_death_star.load("images/death_star.png",gl.RGB8);

    tex_asteroid = Texture2d();
    tex_asteroid.load("images/rock.png",gl.RGB8);
	
    // texture cubeMap for the skybox
	tex_skybox = TextureCubeMap();
    tex_skybox.load(["images/skybox/skybox_milky_way_4k.png", "images/skybox/skybox_milky_way_4k.png", "images/skybox/skybox_4k.png",
     "images/skybox/skybox_4k.png", "images/skybox/skybox_milky_way_4k.png", "images/skybox/skybox_milky_way_4k.png"]).then( update_wgl );

    // Create a mesh cube and his renderer
    prg_skybox = ShaderProgram(sky_vert,sky_frag,"sky");
    sky_rend = Mesh.CubePosOnly().renderer(0,-1,-1,-1);
    
    // Create all the shader programs
    planetShaderProgram = ShaderProgram( vertexShader, fragmentShader, 'planet shader' );
    sunShaderProgram = ShaderProgram(sunVertexShader, sunFragmentShader,'sun shader');

    
    // Create a mesh of a sphere and a renderer
    let planet = Mesh.Sphere(100);
    planet_rend = planet.renderer(1,3,2,-1,-1);
    sun_rend = planet.renderer(1,-1,2,-1,-1);
    
	// Set the radius and the center of the scene
    ewgl.scene_camera.set_scene_radius(scene_radius);
	ewgl.scene_camera.set_scene_center(scene_center);		

    // Asteroid Belt
    // ----------------------------------------------------------------------------------------------------
    // Shader Program for asteroïds
    asteroidShaderProgram = ShaderProgram( asteroidVertexShader, AsteroidFragmentShader, 'Asteroid Shader' );

    // Create a typed array to contain all the 4x4 model matrices of each asteroïd
    const matrixData = new Float32Array(4 * 4 * nbAsteroids);
    // For each asteroïd
    for (let i = 0; i < nbAsteroids; ++i)
    {
        //Compute a matrix model
        var model = Matrix.rotateZ(i) ['*'] (Matrix.translate(getRandomMinMax(38,42),Math.random(),(Math.random()*4)-2)) ['*']
                 (Matrix.scale( 0.05*Math.random())) ['*'] (Matrix.rotateX(getRandomMax(360)));

        //Put the matrix model a the right place in the typed array
        var index = 16 * i;
        matrixData.set(model.data, index);
    }

    // VBO for model matrix of each instance
    const matrixBuffer = VBO(matrixData);

    // Load the .obj mesh and use an instanced renderer (with 4 VBO, to recreate a 4x4 matrix) to get a lot of asteroïds
    Mesh.loadObjFile("rock/rock.obj").then((meshes) =>
    {
        asteroid_rend = meshes[0].instanced_renderer([
           [3, matrixBuffer, 1, 4 * 4, 0 * 4, 4],
           [4, matrixBuffer, 1, 4 * 4, 1 * 4, 4],
           [5, matrixBuffer, 1, 4 * 4, 2 * 4, 4],
           [6, matrixBuffer, 1, 4 * 4, 3 * 4, 4]],
           0, 1, 2);
    });
    // then, the matrice of an instance can be retrieved in a vertex shader with : layout(location=3) in mat4 instanceMatrix;
    // ----------------------------------------------------------------------------------------------------

    // Asteroid belt for Saturn
    const matrixData2 = new Float32Array(4 * 4 * nbAsteroids_saturn);
    for (let i = 0; i < nbAsteroids_saturn; ++i)
    {
        var model2 = Matrix.rotateZ(i) ['*'] (Matrix.translate(getRandomMinMax(4,6),Math.random(),(Math.random()*0.5)-0.25)) ['*']
                 (Matrix.scale( 0.05*Math.random())) ['*'] (Matrix.rotateX(getRandomMax(360)));
        var index2 = 16 * i;
        matrixData2.set(model2.data, index2);
    }

    const matrixBuffer2 = VBO(matrixData2);

    Mesh.loadObjFile("rock/rock.obj").then((meshes) =>
    {
        asteroid_rend2 = meshes[0].instanced_renderer([
           [3, matrixBuffer2, 1, 4 * 4, 0 * 4, 4],
           [4, matrixBuffer2, 1, 4 * 4, 1 * 4, 4],
           [5, matrixBuffer2, 1, 4 * 4, 2 * 4, 4],
           [6, matrixBuffer2, 1, 4 * 4, 3 * 4, 4]],
           0, 1, 2);
    });

    // ATMOSPHERE (GLOW)

    // Create Shader programs
    glowShaderProgram=ShaderProgram(GlowVertexShader,GlowFragmentShader,'glow shader program');

    glow_tex1 = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D,glow_tex1);
    gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,64,64,0,gl.RGBA,gl.UNSIGNED_BYTE,null);

    glow_tex2 = gl.createTexture();
    gl.bindTexture(gl.TEXTURE_2D,glow_tex2);
    gl.texImage2D(gl.TEXTURE_2D,0,gl.RGBA,64,64,0,gl.RGBA,gl.UNSIGNED_BYTE,null);
    // Create FBOs with the linked textures
    fbo1 = gl.createFramebuffer();
    gl.bindFramebuffer(gl.FRAMEBUFFER, fbo1);
    gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, glow_tex1, 0);
    gl.drawBuffers([gl.COLOR_ATTACHMENT0]);
    gl.bindFramebuffer(gl.FRAMEBUFFER,null);

    fbo2 = gl.createFramebuffer();
    gl.bindFramebuffer(gl.FRAMEBUFFER, fbo2);
    gl.framebufferTexture2D(gl.FRAMEBUFFER, gl.COLOR_ATTACHMENT0, gl.TEXTURE_2D, glow_tex2, 0);
    gl.drawBuffers([gl.COLOR_ATTACHMENT0]);
    gl.bindFramebuffer(gl.FRAMEBUFFER,null);

    glow_rend = planet.renderer(1,-1,-1,-1,-1);


    // User interface
    UserInterface.begin();
    UserInterface.use_field_set('H',"Imputs");
    UserInterface.add_label("'+' and '-' to swap camera focus between planets\n'a' and 'z' to speed down and speed up\n");
    UserInterface.end_use();
	UserInterface.end();
}

function getRandomMax(max)
{
	return Math.random() * Math.floor(max);
}

function getRandomMinMax(min, max)
{
	return Math.random() * (max - min) + min;
}

function resize_wgl(w,h)
{
    //let d = Math.pow(2, 3);
    // 
	//fbo1.resize(w/d,h/d);
	//fbo2.resize(w/d,h/d);
	// Faire varier l'intensiter selon la taille
	// glow_intensity = 300 - ((w/100) * (h/100));
}

// -------------------------------------------------------------------------------------------------------------------------------------
//  DRAW
// -------------------------------------------------------------------------------------------------------------------------------------
function draw_wgl()
{	
	gl.clearColor(0,0,0,0);
	gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    // Render skybox
    gl.disable(gl.DEPTH_TEST);
    unbind_texture_cube();
    prg_skybox.bind();
    Uniforms.projectionviewMatrix = ewgl.scene_camera.get_matrix_for_skybox();
    Uniforms.TU = tex_skybox.bind(0);
    sky_rend.draw(gl.TRIANGLES);
    unbind_shader();
    gl.enable(gl.DEPTH_TEST);

    // ATMOSPHERE
    //premiere partie seulement je n'ai pas réussi à faire les passes
    glowShaderProgram.bind();
    gl.bindFramebuffer(gl.FRAMEBUFFER,fbo1);
    Uniforms.uProjectionMatrix = ewgl.scene_camera.get_projection_matrix();
    Uniforms.uViewMatrix = ewgl.scene_camera.get_view_matrix();
    Uniforms.uModelMatrix = Matrix.scale(5.2);
    Uniforms.uColor = Vec3(1,0.5,0.5);
    glow_rend.draw(gl.TRIANGLES);
    gl.bindFramebuffer(gl.FRAMEBUFFER,null);
    unbind_shader();

    let viewMatrix;
    let modelMatrix;
    
    //render death Star
    // Je ne lui ai pas mis de lumiere car la qualité de sa texture etait deja trop basse
    // pour encore lui enlever de la visibilité
    // c'est une death star -> elle a des sources de lumière à sa surface!
    sunShaderProgram.bind();
    Uniforms.uProjectionMatrix = ewgl.scene_camera.get_projection_matrix();
    viewMatrix =  ewgl.scene_camera.get_view_matrix();
    Uniforms.uViewMatrix = viewMatrix;
    if (death_star_pos_y>scene_radius-100)
        avance=0;
    if (death_star_pos_y< -scene_radius+100)
        avance=1;
    if (avance)
        death_star_pos_y += death_star_speed;
    else 
        death_star_pos_y -= death_star_speed;

    modelMatrix = Matrix.translate(death_star_pos_x,death_star_pos_y,0) ['*'] (Matrix.scale( 5 ));
    Uniforms.uModelMatrix = modelMatrix;
    Uniforms.uSampler = tex_death_star.bind(0);
    sun_rend.draw(gl.TRIANGLES);

    // Render space ship
    // Je ne trouvais pas de modèles .json gratuit et je n'ai pas réussi
    // à exporter celui que j'ai fait via blender avec l'addon threejs exporter
    // J'ai supprimé le code ou je faisais tourner la voiture autour de la death star
    // car ca rendait la scene un peu ridicule
    
    // Render Sun
    Uniforms.uProjectionMatrix = ewgl.scene_camera.get_projection_matrix();
    viewMatrix =  ewgl.scene_camera.get_view_matrix();
    Uniforms.uViewMatrix = viewMatrix;
    modelMatrix = Matrix.scale( 7 );
    Uniforms.uModelMatrix = modelMatrix;
    Uniforms.uSampler = tex_sun.bind(0);
    sun_rend.draw(gl.TRIANGLES);
    unbind_shader();
    

    // Render all the planets
    planetShaderProgram.bind();
    Uniforms.uProjectionMatrix = ewgl.scene_camera.get_projection_matrix();
    viewMatrix = ewgl.scene_camera.get_view_matrix();
    Uniforms.uViewMatrix = viewMatrix; 
    let light_matrix = Matrix.mult(viewMatrix, modelMatrix);
    Uniforms.uLightPosition=light_matrix.transform(Vec3(0));

    //Mercury
    modelMatrix = Matrix.rotateZ(ewgl.current_time*4.2*speed+100) ['*'] (Matrix.translate(12,0,0)) ['*'] (Matrix.rotateX(0.1)) ['*']
                         (Matrix.scale( 0.5 )) ['*'] (Matrix.rotateZ(ewgl.current_time*4.2*speed*(88/58.64)));
    mercury_pos =Vec3(modelMatrix ['*'] (Vec4(0,0,0,1)));
    Uniforms.uModelMatrix=modelMatrix;
    light_matrix = Matrix.mult(viewMatrix, modelMatrix);
    Uniforms.uNormalMatrix = light_matrix.inverse3transpose();
    Uniforms.terre=0;
    Uniforms.uSampler = tex_mercury.bind(0);
    planet_rend.draw(gl.TRIANGLES);

    //Venus
    modelMatrix = Matrix.rotateZ(ewgl.current_time*1.62*speed+300) ['*'] (Matrix.translate(17,0,0)) ['*'] (Matrix.rotateX(177)) ['*'] 
                        (Matrix.scale( 1 )) ['*'] (Matrix.rotateZ(ewgl.current_time*1.62*speed*(224.7/-243.01)));
    Uniforms.uModelMatrix = modelMatrix;
    venus_pos =Vec3(modelMatrix ['*'] (Vec4(0,0,0,1)));
    light_matrix = Matrix.mult(viewMatrix, modelMatrix);
    Uniforms.uNormalMatrix = light_matrix.inverse3transpose();
    Uniforms.uSampler = tex_venus.bind(0);
    planet_rend.draw(gl.TRIANGLES);

    //Terre
    modelMatrix = Matrix.rotateZ(ewgl.current_time*1*speed+90) ['*'] (Matrix.translate(22,0,0)) ['*'] (Matrix.rotateX(23)) ['*']
                         (Matrix.scale( 1 )) ['*'] (Matrix.rotateZ(ewgl.current_time*1*speed*365.25));
    earth_pos =Vec3(modelMatrix ['*'] (Vec4(0,0,0,1)));
    Uniforms.terre=1;
    Uniforms.uModelMatrix = modelMatrix;
    light_matrix = Matrix.mult(viewMatrix, modelMatrix);
    Uniforms.uNormalMatrix = light_matrix.inverse3transpose();
    Uniforms.uSampler = tex_earth.bind(0);
    Uniforms.uSampler2 = tex_earth2.bind(1);
    Uniforms.uSampler3 = tex_night_earth.bind(2);
    planet_rend.draw(gl.TRIANGLES);

    //Moon
    modelMatrix = Matrix.rotateZ(ewgl.current_time*1*speed+90) ['*'] (Matrix.translate(22,0,0)) ['*'] (Matrix.rotateX(23)) ['*'] (Matrix.rotateZ(ewgl.current_time*1*speed*13.3)) ['*']
                        (Matrix.translate(2.5,0,0)) ['*'] (Matrix.scale( 0.15 )) ;
    Uniforms.terre=0;
    moon_pos =Vec3(modelMatrix ['*'] (Vec4(0,0,0,1)));
    Uniforms.uModelMatrix = modelMatrix;
    light_matrix = Matrix.mult(viewMatrix, modelMatrix);
    Uniforms.uNormalMatrix = light_matrix.inverse3transpose();
    Uniforms.uSampler = tex_mercury.bind(0); //je prend le skin de mercure qui lui ressemble
    planet_rend.draw(gl.TRIANGLES);

    //Mars
    modelMatrix = Matrix.rotateZ(ewgl.current_time*0.53*speed+120) ['*'] (Matrix.translate(30,0,0)) ['*'] (Matrix.rotateX(25)) ['*']
                         (Matrix.scale( 0.5 )) ['*'] (Matrix.rotateZ(ewgl.current_time*0.53*speed*(24.62/23.93)*689));
    Uniforms.uModelMatrix = modelMatrix;
    mars_pos =Vec3(modelMatrix ['*'] (Vec4(0,0,0,1)));
    light_matrix = Matrix.mult(viewMatrix, modelMatrix);
    Uniforms.uNormalMatrix = light_matrix.inverse3transpose();
    Uniforms.uSampler = tex_mars.bind(0);
    planet_rend.draw(gl.TRIANGLES);

    //Jupiter
    modelMatrix = Matrix.rotateZ(ewgl.current_time*(1/11.87)*speed) ['*'] (Matrix.translate(50,0,0)) ['*'] (Matrix.rotateX(3)) ['*']
                         (Matrix.scale( 3 )) ['*'] (Matrix.rotateZ(ewgl.current_time*(1/11.87)*speed*365.25*11.87*(23.93/9.92)));
    Uniforms.uModelMatrix = modelMatrix;
    jupiter_pos =Vec3(modelMatrix ['*'] (Vec4(0,0,0,1)));
    light_matrix = Matrix.mult(viewMatrix, modelMatrix);
    Uniforms.uNormalMatrix = light_matrix.inverse3transpose();
    Uniforms.uSampler = tex_jupiter.bind(0);
    planet_rend.draw(gl.TRIANGLES);

    //Saturn
    modelMatrix = Matrix.rotateZ(ewgl.current_time*(1/29.45)*speed+150) ['*'] (Matrix.translate(70,0,0)) ['*'] (Matrix.rotateX(27)) ['*']
                        (Matrix.scale( 3 )) ['*'] (Matrix.rotateZ(ewgl.current_time*(1/29.45)*speed*365.25*29.45*(23.93/10.65)));
    Uniforms.uModelMatrix = modelMatrix;
    saturn_pos =Vec3(modelMatrix ['*'] (Vec4(0,0,0,1)));
    light_matrix = Matrix.mult(viewMatrix, modelMatrix);
    Uniforms.uNormalMatrix = light_matrix.inverse3transpose();
    Uniforms.uSampler = tex_saturn.bind(0);
    planet_rend.draw(gl.TRIANGLES);

    //Asteroid belt for Saturn
    unbind_shader();
    asteroidShaderProgram.bind();
    Uniforms.uSampler = tex_asteroid.bind(0);
    Uniforms.uProjectionMatrix = ewgl.scene_camera.get_projection_matrix();
    Uniforms.uViewMatrix = ewgl.scene_camera.get_view_matrix();
    Uniforms.uTimeMatrix = (Matrix.rotateZ(ewgl.current_time*(1/29.45)*speed+150)) ['*'] (Matrix.translate(70,0,0)) ['*'] (Matrix.rotateX(27)) ['*']
                            (Matrix.rotateZ(ewgl.current_time*speed*10));
    if (asteroid_rend2!=null)
        asteroid_rend2.draw(gl.TRIANGLES,nbAsteroids);
    unbind_shader();

    //Uranus
    planetShaderProgram.bind();
    modelMatrix = Matrix.rotateZ(ewgl.current_time*(1/84.07)*speed+250) ['*'] (Matrix.translate(80,0,0)) ['*'] (Matrix.rotateX(98)) ['*']
                         (Matrix.scale( 2 )) ['*'] (Matrix.rotateZ(ewgl.current_time*(1/84.07)*speed*365.25*84.07*(23.94/17.24))) ;
    Uniforms.uModelMatrix = modelMatrix;
    uranus_pos =Vec3(modelMatrix ['*'] (Vec4(0,0,0,1)));
    light_matrix = Matrix.mult(viewMatrix, modelMatrix);
    Uniforms.uNormalMatrix = light_matrix.inverse3transpose();
    Uniforms.uSampler = tex_uranus.bind(0);
    planet_rend.draw(gl.TRIANGLES);

    //Neptune
    modelMatrix = Matrix.rotateZ(ewgl.current_time*(1/164.89)*speed+50) ['*'] (Matrix.translate(110,0,0)) ['*'] (Matrix.rotateX(30)) ['*']
                         (Matrix.scale( 2 )) ['*'] (Matrix.rotateZ(ewgl.current_time*(1/164.89)*speed*365.25*164.89*(23.93/16.11)));
    Uniforms.uModelMatrix = modelMatrix;
    neptune_pos =Vec3(modelMatrix ['*'] (Vec4(0,0,0,1)));
    light_matrix = Matrix.mult(viewMatrix, modelMatrix);
    Uniforms.uNormalMatrix = light_matrix.inverse3transpose();
    Uniforms.uSampler = tex_neptune.bind(0);
    planet_rend.draw(gl.TRIANGLES);

    unbind_shader();


    // Render asteroïds belt
    asteroidShaderProgram.bind();
    Uniforms.uSampler = tex_asteroid.bind(0);
    Uniforms.uProjectionMatrix = ewgl.scene_camera.get_projection_matrix();
    Uniforms.uViewMatrix = ewgl.scene_camera.get_view_matrix();
    Uniforms.uTimeMatrix = Matrix.rotateZ(ewgl.current_time*speed);
    if (asteroid_rend!=null)
        asteroid_rend.draw(gl.TRIANGLES,nbAsteroids);
    unbind_shader();
    
    onkeydown_wgl();

    //set secne center en fonction de l'objet qu'on regarde
    if(focus==0)
        ewgl.scene_camera.set_scene_center(Vec3(0));
    if(focus==1)
        ewgl.scene_camera.set_scene_center(mercury_pos);
    if(focus==2)
        ewgl.scene_camera.set_scene_center(venus_pos);
    if(focus==3)
        ewgl.scene_camera.set_scene_center(earth_pos);
    if(focus==4)
        ewgl.scene_camera.set_scene_center(moon_pos);
    if(focus==5)
        ewgl.scene_camera.set_scene_center(mars_pos);
    if(focus==6)
        ewgl.scene_camera.set_scene_center(jupiter_pos);
    if(focus==7)
        ewgl.scene_camera.set_scene_center(saturn_pos);
    if(focus==8)
        ewgl.scene_camera.set_scene_center(uranus_pos);
    if(focus==9)
        ewgl.scene_camera.set_scene_center(neptune_pos);
}

function mousedown_wgl(ev)
{
    // if you want to use mouse interaction
}

function onkeydown_wgl(k)
{
    // if you want to use keyboard interaction
    if(k=="z")
        speed=speed*10;
    if(k=="a")
        speed=speed/10;
    if(k=="+")
    {
        if(focus==9)
            focus=0;
        else
            focus++;
    }
    if(k=="-")
    {
        if(focus==0)
            focus=9;
        else
            focus--;
    }
        
}

ewgl.launch_3d();
