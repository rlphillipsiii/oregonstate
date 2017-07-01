MODEL
MODEL_VERSION "v1998.8";
DESIGN "Receiver";

/* port names and type */
INPUT S:PIN34 = Reset_In;
INPUT S:PIN43 = Clk_In;
INPUT S:PIN31 = Serial_In;
OUTPUT S:PIN13 = Ldir;
OUTPUT S:PIN19 = Len;
OUTPUT S:PIN6 = Rdir;
OUTPUT S:PIN39 = Ren;

/* timing arc definitions */
Clk_In_Ldir_delay: DELAY Clk_In Ldir;
Clk_In_Len_delay: DELAY Clk_In Len;
Clk_In_Rdir_delay: DELAY Clk_In Rdir;
Clk_In_Ren_delay: DELAY Clk_In Ren;

/* timing check arc definitions */

ENDMODEL
