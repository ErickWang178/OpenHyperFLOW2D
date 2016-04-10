                     OpenHyperFLOW2D

Copyright (C)  1995-2016 by Serge A. Suchkov (sergeas67@gmail.com)
                   Copyright policy: LGPL V3
	    http://github.com/sergeas67/openhyperflow2d	   
	

CFD code for simulation 2D (flat/axisymmetric) transient viscous
compressible multicomponent sub/trans/supersonic reacting gas flows.

include:
	- 2D (flat and axisymmetric) formulation for non stationary Euler/Navier-Stokes equations
	- BC of I, II and III type for any of the dependent variables;
	- no-slip BC on walls;
	- multicomponent flow (4 base components);
	- temperature dependence properties of components;
	- chemical reactions with infinity speed (Zeldovich model);
	- turbulence (RANS/URANS models):
	    + Zero-equation models:
		* Prandtl;
		* van Driest;
		* Escudier;
		* Klebanoff;
		* Smagorinsky-Lilly;
	    + One-equation models:
		* Spalart-Allmaras model;
	    + Two-equations models:
		* Standard (Spalding) k-eps model;
		* Chien k-eps model;
		* Jones-Launder k-eps model;
		* Launder and Sharma, with Yap-correction k-eps model;
		* RNG k-eps model;
	- Tecplot (ascii) 2D output format;
	- Save&Restart simulation state;
	- Serial&Parallel(OpenMP, MPI) version;

OpenHyperFLOW2D it is open source clone of HyperFLOW2D (in-house CFD code)
with reduced functionality, and can be used without restriction for educational
and academic purpose. Reference at the publication is obligatory.

This code is provided entirely free of charge but your donations are welcome.

BTC:1GpTDjxxaSpEKzurqyXuegfR23o6cPAMSu
LTC:Ld219CMcbJThCGQhe8Yam3oqVVWxvKS51i
DASH:XjXmHvTDR7gWJtxNvhReGQoGUj1ZN1oNkR
