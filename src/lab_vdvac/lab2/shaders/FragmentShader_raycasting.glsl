#version 430

layout(location = 0) in vec3 EntryPoint;
layout(location = 1) in vec4 ExitPointCoord;

uniform sampler2D ExitPoints;
uniform sampler3D VolumeTex;
uniform sampler1D TransferFunc;
uniform float     StepSize;
layout(location = 0) out vec4 FragColor;

void main()
{
	// TODO4
	// calculeaza coordonatele pentru accesarea texturii ExistPoints 
	// in functie de ExitPointCoord (componentele x,y si w)
	vec2 ExitFragCoord = vec2(
		(ExitPointCoord.x / ExitPointCoord.w + 1) / 2,
		(ExitPointCoord.y / ExitPointCoord.w + 1) / 2
	);

	//TODO5
	// acceseaza coordonatele texturii ExitPoints in functie de coordonatele de textura
	vec3 ExitPoint = texture(ExitPoints, ExitFragCoord).xyz;
	if (EntryPoint == ExitPoint)
		//daca sunt pe background nu e nevoie sa calculez nimic
		discard;

	// TODO6
	// calculeaza directia razei ca punctul de iesire minus punctul de intrare
	vec3 dir = ExitPoint - EntryPoint;


	// lungimea razei e calculata pentru terminarea razei
	float len = length(dir);

	////TODO7
	// calculeaza vectorul delta cu care se inainteaza pe raza, 
	//in functie de StepSize si de directia razei (directia fiind normalizata)
	vec3 deltaDir = normalize(dir) * StepSize;

	vec3 voxelCoord = EntryPoint;
	vec4 colorAcum = vec4(0);		// culoarea acumulata (Cdst)
	float alphaAcum = 0;           // opacitatea acumulata (Adst)
	float intensity;
	float lengthAcum = 0;
	vec4 colorSample;				// culoarea curenta (Csrc) 
	float alphaSample;				// opacitatea curenta (Asrc)

	vec4 bgColor = vec4(1, 1, 1, 0);	//culoarea de fundal

	for (int i = 0; i < 2000; i++)	//nu calculez mai mult de 2000 culori per raza
	{
		//TODO8
		// determina intensitatea in functie de textura VolumeTex 
		// si de coordonatele voxelului curent
		// Vedeti ca textura intoarce un vec4 si intensity este un float (luati primul canal)
		intensity = texture(VolumeTex, voxelCoord).x;

		//TODO9
		// determina culoarea cu functia de transfer 
		//in functie de textura TransferFunc si de intensitatea voxelului
		colorSample = texture(TransferFunc, intensity);

		if (colorSample.a > 0.0) {
			// se moduleaza opacitatea si culoarea lui colorSample dupa niste formule
			colorSample.a = 1.0 - pow(1.0 - colorSample.a, StepSize * 200.0f);
			colorSample.rgb *= colorSample.a;

			// compunere din fata in spate
			colorAcum.rgb += (1.0 - colorAcum.a) * colorSample.rgb;
			colorAcum.a += (1.0 - colorAcum.a) * colorSample.a;

		}

		////TODO10
		////actualizeaza pozitia lui voxelCoord in funcție de deltaDir
		voxelCoord += deltaDir;

		////TODO11 
		////actualizeaza lungimea parcursa pe raza in functie de pasul de esantionare
		lengthAcum += length(deltaDir);

		if (lengthAcum >= len)
		{
			// s-a terminat daca am iesit cu raza din volum	
			colorAcum.rgb = colorAcum.rgb + (1 - colorAcum.a) * bgColor.rgb;
			break;
		}
		else if (colorAcum.a > 1.0)
		{
			//s-a terminat daca opacitatea acumulata a ajuns la 1
			colorAcum.a = 1.0;
			break;
		}
	}
	FragColor = colorAcum;

	//Numai pentru testare
	//decomenteaza linia de mai jos pentru a vedea punctele de intrare a razelor in volum
	//FragColor = vec4(EntryPoint, 1.0);

	//Numai pentru testare
	//decomenteaza linia de mai jos pentru a vedea punctele de iesire a razelor din volum
	//FragColor = vec4(ExitPoint, 1.0);

}
