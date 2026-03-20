clrlwi r5, r3, 28
extrwi r0, r3, 4, 24
stb r5, 0x0(r4)
extrwi r7, r3, 4, 20
extrwi r6, r3, 4, 16
extrwi r5, r3, 4, 12
stb r0, 0x1(r4)
extrwi r0, r3, 4, 8
li r3, 0x0
stb r7, 0x2(r4)
stb r6, 0x3(r4)
stb r5, 0x4(r4)
stb r0, 0x5(r4)
blr
