%macro colon 2
%2:
	%ifdef ELEM
		dq ELEM
	%else 
		dq 0
    %endif			
    db %1, 0        
	%define ELEM %2	
%endmacro

