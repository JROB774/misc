/*******************************************************************************
 * Extracts wall and floor textures from a WAD file (used by games like DOOM).
 * The Unlicense (Public Domain) <http://unlicense.org/>
 * Authored By Joshua Robertson
 *
*******************************************************************************/

/*////////////////////////////////////////////////////////////////////////////*/

#define STB_IMAGE_WRITE_STATIC
#define STB_IMAGE_WRITE_IMPLEMENTATION

// c library includes
#include <cstdlib>
#include <cstdio> // @TEMP
#include <cstdint>
#include <cstring>
// c++ library includes
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

#include "stb_image_write.h"

enum PlaypalID
{
    DOOM,
    HERETIC,
    HEXEN,
    STRIFE,
    BLASPHEMER,
    HARMONY,
    ACTIONDOOM2
};

constexpr int PLAYPAL_CHANNELS = 3; // RGB format
constexpr int PLAYPAL_ENTRIES = 256;
constexpr int PLAYPAL_SIZE = PLAYPAL_ENTRIES * PLAYPAL_CHANNELS;

constexpr int FLAT_SIZE = 4096;

constexpr int TEXTURE_HEADER_SIZE = 8;

constexpr char STR_NOPLAYPAL[] =
"No PLAYPAL (color palette) found within the WAD!\n"
"Please select a pre-existing PLAYPAL to use for conversion:\n"
"[0] DOOM\n[1] HERETIC\n[2] HEXEN\n[3] STRIFE\n"
"[4] BLASPHEMER\n[5] HARMONY\n[6] ACTION DOOM 2\n";

constexpr const char* PLAYPAL[] = {
    /* DOOM.PAL        */ "00 00 00\n1F 17 0B\n17 0F 07\n4B 4B 4B\nFF FF FF\n1B 1B 1B\n13 13 13\n0B 0B 0B\n07 07 07\n2F 37 1F\n23 2B 0F\n17 1F 07\n0F 17 00\n4F 3B 2B\n47 33 23\n3F 2B 1B\nFF B7 B7\nF7 AB AB\nF3 A3 A3\nEB 97 97\nE7 8F 8F\nDF 87 87\nDB 7B 7B\nD3 73 73\nCB 6B 6B\nC7 63 63\nBF 5B 5B\nBB 57 57\nB3 4F 4F\nAF 47 47\nA7 3F 3F\nA3 3B 3B\n9B 33 33\n97 2F 2F\n8F 2B 2B\n8B 23 23\n83 1F 1F\n7F 1B 1B\n77 17 17\n73 13 13\n6B 0F 0F\n67 0B 0B\n5F 07 07\n5B 07 07\n53 07 07\n4F 00 00\n47 00 00\n43 00 00\nFF EB DF\nFF E3 D3\nFF DB C7\nFF D3 BB\nFF CF B3\nFF C7 A7\nFF BF 9B\nFF BB 93\nFF B3 83\nF7 AB 7B\nEF A3 73\nE7 9B 6B\nDF 93 63\nD7 8B 5B\nCF 83 53\nCB 7F 4F\nBF 7B 4B\nB3 73 47\nAB 6F 43\nA3 6B 3F\n9B 63 3B\n8F 5F 37\n87 57 33\n7F 53 2F\n77 4F 2B\n6B 47 27\n5F 43 23\n53 3F 1F\n4B 37 1B\n3F 2F 17\n33 2B 13\n2B 23 0F\nEF EF EF\nE7 E7 E7\nDF DF DF\nDB DB DB\nD3 D3 D3\nCB CB CB\nC7 C7 C7\nBF BF BF\nB7 B7 B7\nB3 B3 B3\nAB AB AB\nA7 A7 A7\n9F 9F 9F\n97 97 97\n93 93 93\n8B 8B 8B\n83 83 83\n7F 7F 7F\n77 77 77\n6F 6F 6F\n6B 6B 6B\n63 63 63\n5B 5B 5B\n57 57 57\n4F 4F 4F\n47 47 47\n43 43 43\n3B 3B 3B\n37 37 37\n2F 2F 2F\n27 27 27\n23 23 23\n77 FF 6F\n6F EF 67\n67 DF 5F\n5F CF 57\n5B BF 4F\n53 AF 47\n4B 9F 3F\n43 93 37\n3F 83 2F\n37 73 2B\n2F 63 23\n27 53 1B\n1F 43 17\n17 33 0F\n13 23 0B\n0B 17 07\nBF A7 8F\nB7 9F 87\nAF 97 7F\nA7 8F 77\n9F 87 6F\n9B 7F 6B\n93 7B 63\n8B 73 5B\n83 6B 57\n7B 63 4F\n77 5F 4B\n6F 57 43\n67 53 3F\n5F 4B 37\n57 43 33\n53 3F 2F\n9F 83 63\n8F 77 53\n83 6B 4B\n77 5F 3F\n67 53 33\n5B 47 2B\n4F 3B 23\n43 33 1B\n7B 7F 63\n6F 73 57\n67 6B 4F\n5B 63 47\n53 57 3B\n47 4F 33\n3F 47 2B\n37 3F 27\nFF FF 73\nEB DB 57\nD7 BB 43\nC3 9B 2F\nAF 7B 1F\n9B 5B 13\n87 43 07\n73 2B 00\nFF FF FF\nFF DB DB\nFF BB BB\nFF 9B 9B\nFF 7B 7B\nFF 5F 5F\nFF 3F 3F\nFF 1F 1F\nFF 00 00\nEF 00 00\nE3 00 00\nD7 00 00\nCB 00 00\nBF 00 00\nB3 00 00\nA7 00 00\n9B 00 00\n8B 00 00\n7F 00 00\n73 00 00\n67 00 00\n5B 00 00\n4F 00 00\n43 00 00\nE7 E7 FF\nC7 C7 FF\nAB AB FF\n8F 8F FF\n73 73 FF\n53 53 FF\n37 37 FF\n1B 1B FF\n00 00 FF\n00 00 E3\n00 00 CB\n00 00 B3\n00 00 9B\n00 00 83\n00 00 6B\n00 00 53\nFF FF FF\nFF EB DB\nFF D7 BB\nFF C7 9B\nFF B3 7B\nFF A3 5B\nFF 8F 3B\nFF 7F 1B\nF3 73 17\nEB 6F 0F\nDF 67 0F\nD7 5F 0B\nCB 57 07\nC3 4F 00\nB7 47 00\nAF 43 00\nFF FF FF\nFF FF D7\nFF FF B3\nFF FF 8F\nFF FF 6B\nFF FF 47\nFF FF 23\nFF FF 00\nA7 3F 00\n9F 37 00\n93 2F 00\n87 23 00\n4F 3B 27\n43 2F 1B\n37 23 13\n2F 1B 0B\n00 00 53\n00 00 47\n00 00 3B\n00 00 2F\n00 00 23\n00 00 17\n00 00 0B\n00 00 00\nFF 9F 43\nFF E7 4B\nFF 7B FF\nFF 00 FF\nCF 00 CF\n9F 00 9B\n6F 00 6B\nA7 6B 6B\n",
    /* HERETIC.PAL     */ "02 02 02\n02 02 02\n10 10 10\n18 18 18\n1F 1F 1F\n24 24 24\n2C 2C 2C\n30 30 30\n37 37 37\n3F 3F 3F\n46 46 46\n4E 4E 4E\n56 56 56\n5D 5D 5D\n65 65 65\n6C 6C 6C\n74 74 74\n7C 7C 7C\n83 83 83\n8B 8B 8B\n92 92 92\n9A 9A 9A\nA2 A2 A2\nA9 A9 A9\nB1 B1 B1\nB8 B8 B8\nC0 C0 C0\nC8 C8 C8\nCF CF CF\nD2 D2 D2\nD7 D7 D7\nDE DE DE\nE4 E4 E4\nEC EC EC\nF5 F5 F5\nFF FF FF\n32 32 32\n3B 3C 3B\n45 48 44\n4E 50 4D\n58 5D 56\n61 64 5F\n6D 70 68\n74 7B 70\n7D 83 79\n86 8D 82\n90 97 8B\n99 A1 94\nA3 AB 9D\nAC B5 A6\nB5 BD B0\nBD C4 B9\n14 10 24\n18 18 2C\n24 24 3C\n34 34 50\n44 44 60\n58 58 74\n6C 6C 88\n7C 7C 98\n94 94 AC\nA4 A4 B8\nB4 B8 C8\nC0 C4 D0\nD0 D0 D8\nE0 E0 E0\n1B 0F 08\n26 14 0B\n31 1B 0E\n3D 1F 0E\n41 23 12\n4A 25 13\n53 2B 13\n57 2F 17\n5F 33 1B\n67 3B 1F\n73 43 23\n7B 4B 27\n83 53 2F\n8F 5B 33\n97 63 3B\nA0 6C 40\nAF 74 4A\nB4 7E 51\nC0 87 5B\nCC 8F 5D\nD5 97 67\nD8 9F 73\nDC A7 7E\nDF AF 8A\nE3 B7 95\nE6 BE A1\nE9 C6 AC\nED CE B8\nF0 D6 C3\n3E 28 0B\n4B 32 10\n54 3B 17\n5F 43 1E\n67 4B 26\n6E 53 2F\n7B 5F 37\n89 6B 3E\n96 76 4B\nA3 81 54\nAB 89 5C\nB4 92 65\nBC 9A 6D\nC4 A2 75\nCC AA 7D\nD0 B0 85\n25 14 04\n2F 18 04\n39 1C 06\n44 21 04\n4C 24 03\n54 28 00\n61 2F 02\n72 36 00\n7D 3F 06\n8D 4B 09\n9B 53 11\nA2 5F 15\nA9 67 1A\nB4 71 20\nBC 7C 14\nCC 88 18\nDC 94 1C\nEC A0 17\nF4 AC 2F\nFC BB 39\nFC C2 46\nFB C9 53\nFB D0 61\nFB D6 6E\nFB DD 7B\nFA E4 88\n9D 33 04\nAA 41 02\nB9 56 04\nD5 76 04\nEC A4 03\nF8 BE 03\nFF D8 2B\nFF FF 00\n43 00 00\n4F 00 00\n5B 00 00\n67 00 00\n73 00 00\n7F 00 00\n8B 00 00\n9B 00 00\nA7 00 00\nB3 00 00\nBF 00 00\nCB 00 00\nD7 00 00\nE3 00 00\nEF 00 00\nFF 00 00\nFF 34 34\nFF 4A 4A\nFF 5F 5F\nFF 7B 7B\nFF 9B 9B\nFF B3 B3\nFF C9 C9\nFF D7 D7\n3C 0C 58\n50 08 6C\n68 08 80\n80 00 90\n98 00 B0\nB8 00 E0\nD8 2C FC\nE0 78 F0\n25 06 81\n3C 21 93\n52 3D A5\n69 58 B7\n80 74 C9\n97 8F DB\nAD AB ED\nC4 C6 FF\n02 04 29\n02 05 31\n06 08 39\n02 05 41\n02 05 4F\n00 04 58\n00 04 60\n00 04 68\n02 05 79\n02 05 89\n06 09 9F\n0C 10 B8\n20 28 C8\n38 3C DC\n50 50 FD\n50 6C FC\n50 88 FC\n50 A4 FC\n50 C4 FC\n48 DC FC\n50 EC FC\n54 FC FC\n98 FC FC\nBC FC F4\n0B 17 07\n13 23 0B\n17 33 0F\n1F 43 17\n27 53 1B\n2F 63 23\n37 73 2B\n3F 83 2F\n43 93 37\n4B 9F 3F\n53 AF 47\n5B BF 4F\n5F CF 57\n67 DF 5F\n6F EF 67\n77 FF 6F\n17 1F 17\n1B 23 1B\n1F 2B 1F\n23 33 23\n2B 37 2B\n2F 3F 2F\n33 47 33\n3B 4B 37\n3F 53 3B\n43 5B 43\n4B 5F 47\n4F 67 4B\n57 6F 4F\n5B 73 53\n5F 7B 57\n67 83 5F\nFF DF 00\nFF BF 00\nFF 9F 00\nFF 7F 00\nFF 5F 00\nFF 3F 00\nF4 0E 03\n37 00 00\n2F 00 00\n27 00 00\n17 00 00\n0F 0F 0F\n0B 0B 0B\n07 07 07\nFF FF FF\n",
    /* HEXEN.PAL       */ "02 02 02\n04 04 04\n0F 0F 0F\n13 13 13\n1B 1B 1B\n1C 1C 1C\n21 21 21\n27 27 27\n2D 2D 2D\n33 33 33\n39 39 39\n3F 3F 3F\n45 45 45\n4B 4B 4B\n51 51 51\n56 56 56\n5C 5C 5C\n62 62 62\n68 68 68\n70 70 70\n79 79 79\n82 82 82\n8B 8B 8B\n93 93 93\n9D 9D 9D\nA6 A6 A6\nB0 B0 B0\nB9 B9 B9\nC2 C2 C2\nCB CB CB\nD4 D4 D4\nDD DD DD\nE6 E6 E6\n1D 20 1D\n26 28 25\n32 32 32\n3B 3C 3B\n45 48 44\n4E 50 4D\n58 5D 56\n61 64 5F\n6D 70 68\n74 7B 70\n7D 83 79\n86 8D 82\n90 97 8B\n99 A1 94\nA3 AB 9D\nAC B5 A6\nB5 BD B0\nBD C4 B9\n16 1D 16\n1B 24 1B\n1F 2B 1F\n23 33 23\n2B 37 2B\n2F 3F 2F\n33 47 33\n3B 4B 37\n3F 53 3B\n43 5B 43\n4B 5F 47\n4F 67 4B\n57 6F 4F\n5B 73 53\n5F 7B 57\n67 83 5F\n14 10 24\n1E 1A 2E\n28 24 39\n32 2E 43\n3B 39 4E\n45 43 58\n4F 4D 63\n59 57 6D\n63 61 78\n6D 6B 82\n76 76 8D\n80 80 97\n8A 8A A2\n94 94 AC\n3E 28 0B\n4B 32 10\n54 3B 17\n5F 43 1E\n67 4B 26\n6E 53 2F\n7B 5F 37\n89 6B 3E\n96 76 4B\nA3 81 54\nAB 89 5C\nB4 92 65\nBC 9A 6D\nC4 A2 75\nCC AA 7D\nD0 B0 85\n1B 0F 08\n26 14 0B\n31 1B 0E\n3D 1F 0E\n41 23 12\n4A 25 13\n53 2B 13\n57 2F 17\n5F 33 1B\n67 3B 1F\n73 43 23\n7B 4B 27\n83 53 2F\n8F 5B 33\n97 63 3B\nA0 6C 40\nAF 74 4A\nB4 7E 51\nC0 87 5B\nCC 8F 5D\nD5 97 67\nD8 9F 73\nDC A7 7E\nDF AF 8A\nE3 B7 95\n25 14 04\n2F 18 04\n39 1C 06\n44 21 04\n4C 24 03\n54 28 00\n61 2F 02\n72 36 00\n7D 3F 06\n8D 4B 09\n9B 53 11\nA2 5F 15\nA9 67 1A\nB4 71 20\nBC 7C 14\nCC 88 18\nDC 94 1C\nEC A0 17\nF4 AC 2F\nFC BB 39\nFC C2 46\nFB C9 53\nFB D0 61\nFB DD 7B\n02 04 29\n02 05 31\n06 08 39\n02 05 41\n02 05 4F\n00 04 58\n00 04 60\n00 04 68\n04 06 79\n02 05 89\n14 17 98\n26 29 A7\n38 3B B5\n4A 4D C4\n5B 5E D3\n6D 70 E2\n7F 82 F0\n91 94 FF\n1F 04 04\n27 00 00\n2F 00 00\n37 00 00\n43 00 00\n4F 00 00\n5B 00 00\n67 00 00\n73 00 00\n7F 00 00\n8B 00 00\n9B 00 00\nA7 00 00\nB9 00 00\nCA 00 00\nDC 00 00\nED 00 00\nFF 00 00\nFF 2E 2E\nFF 5B 5B\nFF 89 89\nFF AB AB\n14 10 04\n0D 18 09\n11 21 0C\n15 29 0E\n18 32 11\n1C 39 14\n20 41 18\n23 49 1C\n27 50 1F\n2C 56 25\n2E 5F 26\n33 68 2B\n3C 7A 33\n44 8B 3A\n4D 9D 42\n55 AE 49\n5E C0 51\n9D 33 04\nAA 41 02\nB9 56 04\nD5 77 06\nEA 93 05\nFF B2 06\nFF C3 1A\nFF D8 2D\n04 85 04\n08 AF 08\n02 D7 02\n03 EA 03\n2A FC 2A\n79 FF 79\n03 03 B8\n0F 29 DC\n1C 50 E2\n29 77 E9\n36 9E EF\n43 C5 F6\n50 EC FC\nF4 0E 03\nFF 3F 00\nFF 5F 00\nFF 7F 00\nFF 9F 00\nFF C3 1A\nFF DF 00\n2B 0D 40\n3D 0E 59\n5A 0F 7A\n78 10 9C\n95 10 BD\nB2 11 DE\nC5 4A E8\nD7 81 F3\nEA A9 FD\n3D 10 10\n5A 24 21\n76 38 31\n93 4D 42\nB0 61 53\nCC 75 63\n47 35 02\n51 3F 06\n60 48 00\n6C 50 00\n78 58 00\n80 60 00\n95 70 01\nB5 88 03\nD4 A0 04\nFF FF FF\n",
    /* STRIFE.PAL      */ "00 00 00\nE7 E3 E3\nDF DB DB\nD7 D3 D3\nCF CB CB\nC7 C3 C3\nBF BF BF\nB7 B7 B7\nB3 AF AF\nAB A7 A7\nA3 9F 9F\n9B 97 97\n93 93 93\n8B 8B 8B\n83 83 83\n7B 7B 7B\n77 73 73\n6F 6F 6F\n67 67 67\n5F 5F 5F\n57 57 57\n4F 4F 4F\n47 47 47\n43 3F 3F\n3B 3B 3B\n33 33 33\n2B 2B 2B\n23 23 23\n1B 1B 1B\n13 13 13\n0B 0B 0B\n07 07 07\nBB BF B7\nB3 B7 AB\nA7 B3 9F\nA3 AB 93\n9B A7 8B\n93 9F 7F\n8B 9B 77\n83 93 6B\n7F 8F 67\n77 87 5B\n73 83 53\n6B 7B 4B\n67 77 43\n63 6F 3F\n5B 6B 37\n57 63 2F\n53 5F 2B\n4B 57 23\n47 53 1F\n43 4B 1B\n3F 47 17\n3B 3F 13\n33 3B 0F\n2F 33 0B\n2B 2F 07\n27 2B 07\n1F 23 07\n1B 1F 00\n17 17 00\n0F 13 00\n0B 0B 00\n07 07 00\nDB 2B 2B\nCB 23 23\nBF 1F 1F\nAF 1B 1B\nA3 17 17\n93 13 13\n87 0F 0F\n77 0B 0B\n6B 07 07\n5B 07 07\n4F 00 00\n3F 00 00\n33 00 00\n27 00 00\n17 00 00\n0B 00 00\nEB E7 00\nE7 D3 00\nD7 B3 00\nC7 97 00\nB7 7F 00\nA7 67 00\n97 53 00\n87 3F 00\n77 2F 00\n67 23 00\n57 17 00\n47 0B 00\n37 07 00\n27 00 00\n17 00 00\n0B 00 00\nB7 E7 7F\nA3 D7 6F\n8F C7 5F\n7F B7 4F\n6B AB 43\n5B 9B 37\n4B 8B 2B\n3F 7B 23\n2F 6F 1B\n23 5F 13\n17 4F 0B\n0F 43 07\n07 33 07\n00 23 00\n00 13 00\n00 07 00\nC7 CF FF\nB7 BB EF\nA3 AB DB\n97 9B CB\n87 8B BB\n7B 7F AB\n6B 6F 9B\n5F 63 8B\n53 53 7B\n43 47 6B\n37 3B 5B\n2F 2F 4B\n23 23 3B\n17 17 2B\n0F 0F 1B\n00 00 0B\nC7 BF 93\nB3 AB 83\nA7 9B 77\n9B 8B 6F\n8F 7F 63\n83 6F 5B\n77 63 4F\n6B 57 47\n5B 47 3B\n4F 3B 33\n43 2F 2B\n37 27 23\n2B 1B 1B\n1F 13 13\n13 0B 0B\n07 07 00\n8F C3 D3\n7B B3 C3\n6B A7 B7\n5B 9B A7\n4B 8B 9B\n3B 7F 8B\n2F 73 7F\n23 67 73\n1B 5B 63\n13 4F 57\n0B 43 47\n07 37 3B\n00 2B 2B\n00 1F 1F\n00 13 13\n00 07 07\nD3 BF AF\nCB B3 A3\nC3 AB 97\nBF 9F 8F\nB7 97 83\nAF 8F 7B\nAB 87 73\nA3 7B 67\n9B 73 5F\n97 6B 57\n8F 63 4F\n8B 5B 47\n83 53 43\n7B 4B 3B\n77 43 33\n6F 3B 2F\n67 37 27\n63 2F 23\n5B 2B 1F\n53 23 1B\n4F 1F 17\n47 1B 13\n3F 13 0F\n3B 0F 0B\n33 0B 07\n2B 07 07\n27 07 00\n1F 00 00\n1B 00 00\n13 00 00\n0B 00 00\n07 00 00\nD3 C7 BB\nCB BF B3\nC3 B7 AB\nBF AF A3\nB7 A7 9B\nAF 9F 93\nAB 97 8B\nA3 8F 87\n9B 8B 7F\n97 83 77\n8F 7B 6F\n87 73 6B\n83 6B 63\n7B 67 5F\n73 5F 57\n6F 57 53\n67 53 4B\n5F 4B 47\n5B 43 3F\n53 3F 3B\n4F 37 33\n47 33 2F\n3F 2B 2B\n3B 27 27\n33 23 1F\n2B 1B 1B\n27 17 17\n1F 13 13\n17 0F 0F\n13 0B 0B\n0B 07 07\n07 07 00\nEF EF 00\nE7 D7 00\nE3 BF 00\nDB AB 00\nD7 97 00\nD3 83 00\nCB 6F 00\nC7 5B 00\nBF 4B 00\nBB 3B 00\nB7 2B 00\nFF 00 00\nDF 00 00\nBF 00 00\n9F 00 00\n7F 00 00\n00 00 00\n8B C7 67\n6B AB 4B\n4F 8F 37\n37 73 23\n23 57 13\n13 3F 0B\nD7 DF FF\nBB CB F7\n8F A7 DB\n63 83 C3\n3F 5B A7\nCB CB CB\nD7 D7 D7\nDF DF DF\nEB EB EB\n",
    /* BLASPHEMER.PAL  */ "00 00 00\n02 02 02\n10 10 10\n18 18 18\n1F 1F 1F\n24 24 24\n2C 2C 2C\n30 30 30\n37 37 37\n3F 3F 3F\n46 46 46\n4E 4E 4E\n56 56 56\n5D 5D 5D\n65 65 65\n6C 6C 6C\n74 74 74\n7C 7C 7C\n83 83 83\n8B 8B 8B\n92 92 92\n9A 9A 9A\nA2 A2 A2\nA9 A9 A9\nB1 B1 B1\nB8 B8 B8\nC0 C0 C0\nC8 C8 C8\nCF CF CF\nD2 D2 D2\nD7 D7 D7\nDE DE DE\nE4 E4 E4\nEC EC EC\nF5 F5 F5\nFF FF FF\n32 32 32\n3B 3C 3B\n45 48 44\n4E 50 4D\n58 5D 56\n61 64 5F\n6D 70 68\n74 7B 70\n7D 83 79\n86 8D 82\n90 97 8B\n99 A1 94\nA3 AB 9D\nAC B5 A6\nB5 BD B0\nBD C4 B9\n14 10 24\n1C 18 2C\n28 24 38\n30 2C 44\n3C 38 4C\n44 44 58\n50 4C 64\n58 58 6C\n64 60 78\n6C 6C 80\n74 74 8C\n80 80 94\n88 88 A0\n94 94 AC\n17 0D 07\n1D 11 09\n23 15 0B\n29 19 0E\n2F 1D 11\n35 21 14\n3B 26 17\n41 2B 1A\n47 2F 1E\n4D 34 21\n53 39 25\n59 3E 2A\n5F 44 2E\n65 49 33\n6B 4F 37\n71 54 3C\n77 5A 41\n7D 60 47\n83 66 4C\n89 6C 52\n8F 72 58\n95 78 5E\n9B 7F 65\nA1 85 6B\nA7 8C 72\nAD 92 79\nB3 99 80\nB9 A0 87\nBF A7 8F\n3C 28 0C\n46 2F 10\n50 37 15\n5A 3F 1A\n63 47 1F\n6D 4F 26\n77 58 2D\n81 61 34\n8B 6A 3C\n95 73 45\n9F 7D 4E\nA9 86 57\nB2 90 62\nBC 9B 6D\nC6 A5 78\nD0 B0 84\n21 08 03\n2C 0C 05\n37 11 07\n42 17 09\n4D 1D 0C\n58 24 0E\n63 2C 12\n6E 34 15\n79 3D 19\n85 46 1E\n90 50 22\n9B 5B 27\nA6 66 2C\nB1 71 32\nBC 7D 38\nC7 89 3E\nD2 96 44\nDD A3 4B\nE0 AA 52\nE2 B1 58\nE5 B8 5F\nE8 BF 66\nEB C7 6C\nEE CE 73\nF0 D5 79\nF3 DC 80\n9D 33 04\nAA 41 02\nB9 56 04\nD5 76 04\nEC A4 03\nF8 BE 03\nFF D8 2B\nFF FF 00\n43 00 00\n4F 00 00\n5B 00 00\n67 00 00\n73 00 00\n7F 00 00\n8B 00 00\n9B 00 00\nA7 00 00\nB3 00 00\nBF 00 00\nCB 00 00\nD7 00 00\nE3 00 00\nEF 00 00\nFF 00 00\nFF 34 34\nFF 4A 4A\nFF 5F 5F\nFF 7B 7B\nFF 9B 9B\nFF B3 B3\nFF C9 C9\nFF D7 D7\n3C 10 10\n51 1E 1C\n65 2D 28\n7A 3B 34\n8E 49 40\nA3 57 4C\nB7 66 58\nCC 74 64\n3B 1F 23\n52 2D 34\n68 3B 48\n7F 4B 5D\n95 5C 73\nAC 6D 8B\nC2 80 A4\nD9 93 BE\n00 04 28\n02 07 33\n06 0A 3E\n0A 0F 49\n0F 14 53\n16 1A 5E\n1D 21 69\n25 29 74\n2E 32 7F\n39 3C 8A\n44 47 95\n50 52 A0\n5D 5F AA\n6C 6D B5\n7B 7B C0\n8B 8B CB\n96 96 D1\nA2 A2 D8\nAE AE DF\nB9 B9 E5\nC4 C4 EB\nD0 D0 F2\nDC DC F8\nE7 E7 FF\n0E 1A 00\n14 24 01\n1B 2F 03\n22 3A 06\n29 45 09\n31 50 0D\n38 5B 12\n41 66 17\n49 71 1D\n52 7C 24\n5B 87 2C\n65 92 34\n6F 9D 3D\n79 A8 47\n83 B3 51\n8E BE 5C\n11 14 04\n1A 1E 0D\n22 27 15\n2A 30 1E\n33 39 26\n3B 42 2E\n43 4B 37\n4B 55 3F\n54 5E 48\n5C 67 50\n64 70 59\n6D 79 61\n75 83 6A\n7D 8C 72\n85 95 7B\n8E 9E 83\nFF DF 00\nFF BF 00\nFF 9F 00\nFF 7F 00\nFF 5F 00\nFF 3F 00\nF4 0E 03\n37 00 00\n2F 00 00\n27 00 00\n17 00 00\n0F 0F 0F\n0B 0B 0B\n07 07 07\nFF FF FF\n",
    /* HARMONY.PAL     */ "00 00 00\nF0 F0 F0\nE8 E8 E8\nE0 E0 E0\nD9 D9 D9\nD1 D1 D1\nC9 C9 C9\nC1 C1 C1\nBA BA BA\nB2 B2 B2\nAA AA AA\nA2 A2 A2\n9B 9B 9B\n93 93 93\n8B 8B 8B\n83 83 83\n7C 7C 7C\n74 74 74\n6C 6C 6C\n64 64 64\n5C 5C 5C\n55 55 55\n4D 4D 4D\n45 45 45\n3D 3D 3D\n36 36 36\n2E 2E 2E\n26 26 26\n1E 1E 1E\n17 17 17\n0F 0F 0F\n07 07 07\nFF FF FF\nFF EB DC\nFF D7 BE\nFF C8 9B\nFF B4 7D\nFF A5 5A\nFF 91 3C\nFF 82 19\nF5 73 19\nEB 6E 0F\nE1 69 0F\nD7 5F 0A\nCD 5A 0A\nC3 50 00\nB9 46 00\nAF 46 00\nAA 41 00\nA0 37 23\n96 32 00\n87 28 00\n7D 28 00\n73 23 13\n6E 23 00\n64 1E 00\n5A 19 00\n50 14 00\n46 14 00\n3C 0F 00\n32 0A 00\n28 05 00\n14 00 00\n0A 00 00\nFF 3C 00\nEF 38 00\nE0 34 00\nD0 30 00\nC0 2C 00\nB1 28 00\nA1 24 00\n91 20 00\n82 1C 00\n72 18 00\n62 14 00\n53 10 00\n43 0C 00\n33 08 00\n24 04 00\n14 00 00\nEB E6 00\nE6 D2 00\nD7 B4 00\nC8 97 00\nB9 7D 00\nAA 73 00\n96 5F 00\n87 4B 00\n78 3C 00\n69 2D 00\n5A 23 00\n50 1E 00\n46 19 00\n37 0F 00\n28 05 00\n0A 00 00\nA0 C8 A0\n91 BF 91\n82 B6 82\n72 AE 72\n63 A5 63\n58 98 58\n4F 89 4F\n46 7A 46\n3E 6A 3E\n35 5B 35\n2C 4C 2C\n23 3D 23\n1A 2E 1A\n12 1E 12\n09 0F 09\n00 00 00\nEB 50 EB\nDC 3C DC\nCD 28 CD\nBE 14 BE\nAF 0A AF\nA0 00 A0\n91 00 91\n82 00 82\n73 00 73\n64 00 64\n55 00 55\n4B 00 4B\n41 00 41\n32 00 32\n23 00 23\n14 00 14\nDC BE A8\nD5 B0 96\nCD A2 83\nC6 95 71\nBE 87 5E\nB7 79 4C\nA6 6D 43\n94 61 3B\n81 55 34\n6F 49 2D\n5C 3C 25\n4A 30 1E\n37 24 16\n25 18 0F\n12 0C 07\n00 00 00\nC8 C8 FF\nA0 AA FF\n95 9F EF\n89 93 DF\n7E 88 CF\n72 7C BF\n67 71 AF\n5B 65 9F\n50 5A 8F\n45 4F 7E\n39 43 6E\n2E 38 5E\n22 2C 4E\n17 21 3E\n0B 15 2E\n00 0A 1E\nFF EB E1\nFF E1 D2\nFF DC C8\nFF D2 BE\nFF D2 B4\nFF C8 AA\nFF BE 9B\nFF BE 96\nFF B4 82\nFA AA 7D\nF0 A5 73\nE6 9B 6E\nE1 96 64\nD7 8C 5A\nD4 82 4C\nD0 79 3F\nCD 70 32\nBF 68 2E\nB1 61 2B\nA4 59 28\n96 52 24\n88 4A 21\n7B 43 1E\n6D 3C 1A\n5F 34 17\n52 2D 14\n44 25 11\n37 1E 0D\n29 16 0A\n1B 0F 07\n0E 07 03\n00 00 00\nDC BE A8\nD5 B8 A3\nCE B2 9D\nC7 AC 98\nC0 A5 92\nB9 9F 8D\nB1 99 87\nAA 93 82\nA3 8D 7D\n9C 87 77\n95 81 72\n8E 7B 6C\n87 74 67\n80 6E 62\n79 68 5C\n72 62 57\n6A 5C 51\n63 56 4C\n5C 50 46\n55 4A 41\n4E 43 3C\n47 3D 36\n40 37 31\n39 31 2B\n32 2B 26\n2B 25 21\n23 1F 1B\n1C 19 16\n15 12 10\n0E 0C 0B\n07 06 05\n00 00 00\nF0 F0 C8\nE6 D7 8C\nE3 BF 00\nDB AB 00\nD7 97 00\nD3 83 00\nCB 6F 00\nC8 5A 00\nBE 46 00\nBE 32 00\nBE 28 1E\nC8 78 FF\nA0 50 FF\n78 28 E1\n64 14 BE\n50 0A 8C\n00 00 00\n6E B4 E6\n37 96 B9\n23 82 AA\n0F 64 8C\n00 50 78\n00 41 64\nC8 DF C8\nA0 D2 8C\n82 BE 64\n50 A0 46\n3C 78 46\nB4 B4 B4\nC8 C8 C8\nDF DF DF\nEB EB EB\n",
    /* ACTIONDOOM2.PAL */ "00 00 00\n1F 17 0B\n17 0F 07\n4B 4B 4B\nFF FF FF\n1B 1B 1B\n13 13 13\n0B 0B 0B\n07 07 07\n2F 37 1F\n23 2B 0F\n17 1F 07\n0F 17 00\n4F 3B 2B\n47 33 23\n3F 2B 1B\nFF B6 B6\nFF A6 A6\nFF 9E 9E\nFF 8E 8E\nFF 84 84\nFF 79 79\nFF 6B 6B\nFF 5F 5F\nFF 54 54\nFF 4B 4B\nFF 40 40\nFF 3A 3A\nFF 2E 2E\nFF 24 24\nFF 19 19\nFF 13 13\nFD 09 09\nF9 05 05\nF3 00 00\nDC 00 00\nC9 00 00\nBE 00 00\nAA 00 00\n9D 00 00\n8C 00 00\n7F 00 00\n6E 00 00\n6B 00 00\n61 00 00\n4D 00 00\n46 00 00\n43 00 00\nFF EB DF\nFF E3 D3\nFF DB C7\nFF D3 BB\nFF CF B3\nFF C7 A7\nFF BF 9B\nFF BB 93\nFF B3 83\nF7 AB 7B\nEF A3 73\nE7 9B 6B\nDF 93 63\nD7 8B 5B\nCF 83 53\nCB 7F 4F\nBF 7B 4B\nB3 73 47\nAB 6F 43\nA3 6B 3F\n9B 63 3B\n8F 5F 37\n87 57 33\n7F 53 2F\n77 4F 2B\n6B 47 27\n5F 43 23\n53 3F 1F\n4B 37 1B\n3F 2F 17\n33 2B 13\n2B 23 0F\nEF EF EF\nE7 E7 E7\nDF DF DF\nDB DB DB\nD3 D3 D3\nCB CB CB\nC7 C7 C7\nBF BF BF\nB7 B7 B7\nB3 B3 B3\nAB AB AB\nA7 A7 A7\n9F 9F 9F\n97 97 97\n93 93 93\n8B 8B 8B\n83 83 83\n7F 7F 7F\n77 77 77\n6F 6F 6F\n6B 6B 6B\n63 63 63\n5B 5B 5B\n57 57 57\n4F 4F 4F\n47 47 47\n43 43 43\n3B 3B 3B\n37 37 37\n2F 2F 2F\n27 27 27\n23 23 23\nBC FF AC\nAD FF 99\n9E FF 86\n8F FF 74\n81 FF 62\n72 FF 4F\n63 F5 40\n5B DF 3A\n52 C9 34\n48 B1 2E\n3D 97 27\n33 7D 20\n29 65 1A\n1E 4B 13\n15 35 0E\n0D 21 08\nBF A7 8F\nB7 9F 87\nAF 97 7F\nA7 8F 77\n9F 87 6F\n9B 7F 6B\n93 7B 63\n8B 73 5B\n83 6B 57\n7B 63 4F\n77 5F 4B\n6F 57 43\n67 53 3F\n5F 4B 37\n57 43 33\n53 3F 2F\n9F 83 63\n8F 77 53\n83 6B 4B\n77 5F 3F\n67 53 33\n5B 47 2B\n4F 3B 23\n43 33 1B\n7B 7F 63\n6F 73 57\n67 6B 4F\n5B 63 47\n53 57 3B\n47 4F 33\n3F 47 2B\n37 3F 27\nFE FE 72\nE0 E0 54\nC5 C5 39\nAA AA 1E\n8F 8F 03\n73 73 00\n5A 5A 00\n42 42 00\nFF FF FF\nFF DA DA\nFF BA BA\nFF 9B 9B\nFF 7A 7A\nFF 5F 5F\nFF 3E 3E\nFF 1F 1F\nED 07 07\nE8 02 02\nE3 00 00\nD5 00 00\nC9 00 00\nBE 00 00\nB1 00 00\nA7 00 00\n99 00 00\n89 00 00\n7F 00 00\n71 00 00\n64 00 00\n5A 00 00\n4D 00 00\n43 00 00\n8D B6 E8\n78 AA E8\n64 9F E8\n50 94 E8\n3D 8A E8\n29 7E E8\n15 74 E8\n04 6A E8\n00 60 E8\n00 54 D7\n00 44 B4\n00 34 92\n00 26 72\n00 16 4F\n00 09 31\n00 00 17\nFF FF FF\nFF EB DB\nFF D7 BB\nFF C7 9B\nFF B3 7B\nFF A3 5B\nFF 8F 3B\nFF 7F 1B\nF3 73 17\nEB 6F 0F\nDF 67 0F\nD7 5F 0B\nCB 57 07\nC3 4F 00\nB7 47 00\nAF 43 00\nFF FF FF\nFF FF D7\nFF FF B3\nFF FF 8F\nFF FF 6B\nFF FF 47\nFF FF 23\nFF FF 00\nA7 3F 00\n9F 37 00\n93 2F 00\n87 23 00\n4F 3B 27\n43 2F 1B\n37 23 13\n2F 1B 0B\n00 00 53\n00 00 47\n00 00 3B\n00 00 2F\n00 00 23\n00 00 17\n00 00 0B\n00 00 00\nFF 9F 43\nFF E7 4B\nFF 7B FF\nFF 00 FF\nCF 00 CF\n9F 00 9B\n6F 00 6B\nA7 6B 6B\n"
};
inline uint32_t Swap32 (uint32_t _x)
{
    return static_cast<uint32_t>(((_x << 24)|((_x << 8) & 0x00FF0000)|((_x >> 8) & 0x0000FF00)|(_x >> 24)));
}

inline std::vector<std::string> StrToken (const std::string& _str, std::string _delim)
{
    std::vector<std::string> tokens;
    std::string::size_type start = 0, end = 0;

    while (end != std::string::npos) {
        end = _str.find_first_of(_delim, start);
        std::string token = _str.substr(start, end-start);
        if (token.length() > 0) { tokens.push_back(token); }
        start = end + 1;
    }

    return tokens;
}

struct Color
{
    std::uint8_t r, g, b;
};

struct WAD
{
    #pragma pack(push, 1)
    struct Header
    {
        char identification[4];
        std::int32_t numlumps;
        std::int32_t infotableofs;
    };
    #pragma pack(pop)

    #pragma pack(push, 1)
    struct Entry
    {
        std::int32_t filepos;
        std::int32_t size;
        char name[8];
    };
    #pragma pack(pop)

    Header header;
    std::vector<Entry> directory;

    bool Extract (std::string _filename)
    {
        // opens the WAD file for binary reading
        std::ifstream file(_filename, std::ios::binary);
        if (!file.is_open()) { return false; }

        // reads in the header from the WAD
        file.read((char*)&header, sizeof(Header));
        // flips the endianness to big, if neccessary
        #if JLIB_BYTEORDER == JLIB_BIG_ENDIAN
        header.numlumps = Swap32(header.numlumps);
        header.infotableofs = Swap32(header.infotableofs);
        #endif

        // allocates memory for all directory entries
        directory.resize(header.numlumps);
        // locates the directory and begins reading in entries
        file.seekg(header.infotableofs);
        for (auto& entry: directory) {
            file.read((char*)&entry, sizeof(Entry));
            // flips the endianness to big, if neccessary
            #if JLIB_BYTEORDER == JLIB_BIG_ENDIAN
            entry.filepos = Swap32(entry.filepos);
            entry.size = Swap32(entry.size);
            #endif
        }

        // data structure to store playpal color information
        std::vector<Color> playpal;
        playpal.reserve(PLAYPAL_ENTRIES);

        // if available, get the PLAYPAL lump's first palette
        auto playpalentry = std::find_if(directory.begin(), directory.end(), \
        [](auto _lump)->bool { return (std::strncmp(_lump.name, "PLAYPAL", 8) == 0); });

        // if the PLAYPL was found...
        if (playpalentry != directory.end()) {
            // load the PLAYPAL information into a usable data format
            file.seekg(playpalentry->filepos);
            std::uint8_t buffer[PLAYPAL_SIZE];
            file.read((char*)&buffer, PLAYPAL_SIZE);
            for (int i = 0; i < PLAYPAL_SIZE; i += PLAYPAL_CHANNELS) {
                playpal.push_back(Color { buffer[i], buffer[i+1], buffer[i+2] });
            }
        }
        // if no PLAYPAL was found...
        else {
            // ask the user which predefined one they would like to use
            int input = -1;
            std::cout << STR_NOPLAYPAL;
            while (input < DOOM || input > ACTIONDOOM2) {
                std::cout << "> ";
                std::cin >> input;
            }

            // load the PLAYPAL information into a usable data format
            std::istringstream playpaldata(PLAYPAL[input]);
            std::string line;
            while (std::getline(playpaldata, line)) {
                auto tokens = StrToken(line);
                playpal.push_back(Color {
                    (std::uint8_t)std::stoul(tokens.at(0), nullptr, 16),
                    (std::uint8_t)std::stoul(tokens.at(1), nullptr, 16),
                    (std::uint8_t)std::stoul(tokens.at(2), nullptr, 16)
                });
            }
        }

        // finds the start and markers for all the flat lumps (ceilings and floors)
        auto fstart = std::find_if(directory.begin(), directory.end(), \
        [](auto _lump)->bool { return ((std::strncmp(_lump.name, "F_START", 7) == 0) || \
        (std::strncmp(_lump.name, "FF_START", 8) == 0)); });
        auto fend = std::find_if(directory.begin(), directory.end(), \
        [](auto _lump)->bool { return ((std::strncmp(_lump.name, "F_END", 5) == 0) || \
        (std::strncmp(_lump.name, "FF_END", 6) == 0)); });

        // if both were found then flats can be searched for in the WAD
        if (fstart != directory.end() && fend != directory.end()) {
            for (auto flat = fstart+1; flat != fend; ++flat) {
                // if the lump is not of the right size then it is not a valid flat
                if (flat->size == FLAT_SIZE) {
                    // locate the flat lump and extract the data
                    file.seekg(flat->filepos);
                    std::uint8_t buffer[FLAT_SIZE];
                    file.read((char*)&buffer, FLAT_SIZE);
                    // convert the palettised data into RGB color data
                    std::uint8_t output[FLAT_SIZE * PLAYPAL_CHANNELS];
                    for (int i = 0; i < FLAT_SIZE; ++i) {
                        output[(i*3)+0] = playpal.at(buffer[i]).r;
                        output[(i*3)+1] = playpal.at(buffer[i]).g;
                        output[(i*3)+2] = playpal.at(buffer[i]).b;
                    }
                    // @TEMP: save the data as a PNG image using stb
                    std::string outputname = "output\\";
                    outputname += std::tmpnam(nullptr);
                    outputname += ".png";
                    stbi_write_png(outputname.c_str(), 64, 64, 3, output, 64 * 3);
                }
            }
        }

        // finds the start and markers for all the texture lumps (walls and patches)
        auto pstart = std::find_if(directory.begin(), directory.end(), \
        [](auto _lump)->bool { return ((std::strncmp(_lump.name, "P_START", 7) == 0) || \
        (std::strncmp(_lump.name, "PP_START", 8) == 0)); });
        auto pend = std::find_if(directory.begin(), directory.end(), \
        [](auto _lump)->bool { return ((std::strncmp(_lump.name, "P_END", 5) == 0) || \
        (std::strncmp(_lump.name, "PP_END", 6) == 0)); });

        // if both were found then textures can be searched for in the WAD
        if (pstart != directory.end() && pend != directory.end()) {
            for (auto patch = pstart+1; patch != pend; ++patch) {
                // do not attempt to convert potential marker lumps
                if (patch->size != 0) {
                    // locate the patch lump and extract the data
                    std::vector<std::uint8_t> buffer;
                    // ...
                    // convert the palettised data into RGB color data
                    // ...
                    // @TEMP: save the data as a PNG image using stb
                    std::string outputname = "output\\";
                    outputname += std::tmpnam(nullptr);
                    outputname += ".png";
                    stbi_write_png(outputname.c_str(), 64, 64, 3, output, 64 * 3);
                }
            }
        }

        // extract sprites (actors, items, etc.)
        // ...

        // extract the extra data (title, HUD, etc.)
        // ...

        // closes the file after use
        file.close();
        return true;
    }
};

int main (int _argc, char* _argv[])
{
    // do not run if no WADs have been passed in
    if (_argc <= 1) {
        std::cout << "No WADs given to read!" << std::endl;
        std::cin.get();
        return EXIT_FAILURE;
    }

    // data structure loosely following WAD format
    WAD wad;

    // reads and extracts image data from all WADs passed
    std::cout << "Beginning WAD extraction..." << std::endl;
    for (int i = 1; i < _argc; ++i) {
        if (wad.Extract(_argv[i])) { std::cout << "'" << _argv[i] << "' complete!" << std::endl; }
        else { std::cout << "'" << _argv[i] << "' failed!" << std::endl; }
    }
    std::cout << "WAD extraction complete!" << std::endl;

    // exit successfully on completion
    std::cin.get();
    return EXIT_SUCCESS;
}

/*////////////////////////////////////////////////////////////////////////////*/

/*******************************************************************************
 * This is free and unencumbered software released into the public domain.
 *
 * Anyone is free to copy, modify, publish, use, compile, sell, or
 * distribute this software, either in source code form or as a compiled
 * binary, for any purpose, commercial or non-commercial, and by any means.
 *
 * In jurisdictions that recognize copyright laws, the author or authors
 * of this software dedicate any and all copyright interest in the
 * software to the public domain. We make this dedication for the benefit
 * of the public at large and to the detriment of our heirs and
 * successors. We intend this dedication to be an overt act of
 * relinquishment in perpetuity of all present and future rights to this
 * software under copyright law.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * For more information, please refer to <http://unlicense.org/>
 *
*******************************************************************************/
