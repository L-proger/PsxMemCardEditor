#include "SjisEncoding.h"


unsigned short sjischar[]=
{
0x8140,		/*   */
0x8149,		/* ! */
0x8168,		/* " */
0x8194,		/* # */
0x8190,		/* $ */
0x8193,		/* % */
0x8195,		/* & */
0x8166,		/* ' */
0x8169,		/* ( */
0x816a,		/* ) */
0x8196,		/* * */
0x817b,		/* + */
0x8143,		/* , */
0x817c,		/* - */
0x8144,		/* . */
0x815e,		/* / */
0x8146,		/* : */
0x8147,		/* ; */
0x8171,		/* < */
0x8181,		/* = */
0x8172,		/* > */
0x8148,		/* ? */
0x8197,		/* @ */
0x816d,		/* [ */
0x818f,		/* \ */
0x816e,		/* ] */
0x814f,		/* ^ */
0x8151,		/* _ */
0x8165,		/* ` */
0x816f,		/* { */
0x8162,		/* | */
0x8170,		/* } */
0x8150		/* ~ */
};

char ascchar[]=
{
' ' ,
'!' ,
'"' ,
'#' ,
'$' ,
'%' ,
'&' ,
'"' ,
'(' ,
')' ,
'*' ,
'+' ,
',' ,
'-' ,
'.' ,
'/' ,
':' ,
';' ,
'<' ,
'=' ,
'>' ,
'?' ,
'@' ,
'[' ,
'\\' ,
']' ,
'^' ,
'_' ,
'`' ,
'{' ,
'|' ,
'}' ,
'~'
};


void sjisToAscii(const std::uint8_t* sjisString, char* asciiString) {
    while(true){
        if(sjisString[0] == 0 || sjisString[1] == 0){
            break;
        }
        std::uint16_t sjisCode = sjisString[1] | (sjisString[0] << 8);

        char ascii = 0xff;
        if ((sjisCode >= 0x8260) && (sjisCode <= 0x8279)){ //A..Z
            ascii = (sjisCode - 0x8260) + 65;
        } else if ((sjisCode >= 0x8281) && (sjisCode <= 0x829A)){ //a..z
            ascii = (sjisCode - 0x8281) + 97;
        } else if ((sjisCode >= 0x824F) && (sjisCode <= 0x8258)) { //0..9
            ascii = (sjisCode - 0x824F) + 48;
        } else {
            for(int i = 0; i < 33; ++i){
                if (sjischar[i] ==  sjisCode){
                    ascii = ascchar[i];
                }
            }
        }

        *asciiString = ascii;
        sjisString += 2;
        asciiString++;
    }
    *asciiString = 0;
}
