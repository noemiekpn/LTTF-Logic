--------------------------------------------------------------------------------	
	A Link To The Future
	Projeto de Inteligência Artificial PUC-Rio
	Agente lógico aplicado a jogo de tema Zelda 
	
	Autores: Gabriel Barros & Noemie Nakamura
	Data: Maio 2014
--------------------------------------------------------------------------------
	DESCRIÇÃO
--------------------------------------------------------------------------------			
	
--------------------------------------------------------------------------------
	ARQUIVOS DE TEXTO
--------------------------------------------------------------------------------		
	Para o mapa e informações quanto aos objetos do mapa, foram utilizados
	arquivos .TXT.
	
	------------------------------------------------------------
		MAPA
	------------------------------------------------------------
	Este arquivo contém o desenho do mapa com cada letra representando um tipo
	de terreno, entre eles,
		
		f - Floresta
		g - Grama
	
	------------------------------------------------------------
		OBJETOS DO MAPA
	------------------------------------------------------------
	Este arquivo contém os tipos de objetos e a quantidade deles. Sua formatação
	se dá da seguinte forma:
	
	1. Os tipos dos objetos são representados com letras maiúsculas pertencentes
	ao seguinte conjunto:
		
							{B, E, V, F, M, C, R}
	Seus siginificados são:						
	B - Buraco
	E - Inimigo
	V - Vortex espacial
	F - Master Sword falsa
	M - Master Sword verdadeira
	C - Coração
	R - Rúpia
	
	2. O número de objetos deve ser um inteiro positivo.
	3. A ordem deve ser: <tipo_do_objeto> <numero_de_objetos>.
	
--------------------------------------------------------------------------------
	BIBLIOTECAS
--------------------------------------------------------------------------------		
	Neste projeto foram utilizadas as bibliotecas:
	- Allegro 5
	- SWI-Prolog C++

	------------------------------------------------------------
		ALLEGRO 5
	------------------------------------------------------------
	Baixe Allegro 5 no link a seguir:
	https://www.allegro.cc/files/
	
	Escolha o arquivo .ZIP de acordo com seu compilador.
	O guia de instalação pode ser encontrado em:
	
	http://wiki.allegro.cc/index.php?title=Allegro_5#Installing_Tutorials
	
	------------------------------------------------------------
		SWI-PROLOG
	------------------------------------------------------------
	Baixe SWI-Prolog para C++ 5 no link a seguir:
	http://www.swi-prolog.org/download/stable

	Para configurá-lo no ambiente Windows, acrescente o diretório bin do
	programa à variável PATH do sistema.
	
	Para configurá-lo no Visual Studio:
	1. Acrescente o diretório include nas propriedades do projeto, em:
		
		C/C++ > General > Additional Include Directories
	
	2. Acrescente o diretório lib nas propriedades do projeto, em:
		
		Linker > General > Additional Library Directories
	
	3. Acrescente libswipl.dll.a nas propriedades do projeto, em:
		
		Linker > Input > Additional Dependencies
	
--------------------------------------------------------------------------------
	
