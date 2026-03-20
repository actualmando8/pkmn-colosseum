stwu r1, -0x10(r1)
mflr r0
stw r0, 0x14(r1)
lha r0, 0x6(r4)
lis r5, lbl_803A9E40@ha
addi r5, r5, lbl_803A9E40@l
cmpwi r0, 0xc7f
beq @8005F648
bge @8005EB70
cmpwi r0, 0xc39
beq @8005F210
bge @8005E9CC
cmpwi r0, 0xc16
beq @8005EFF4
bge @8005E900
cmpwi r0, 0xc02
beq @8005EEDC
bge @8005E8A0
cmpwi r0, 0xbf8
bge @8005E870
cmpwi r0, 0xbf3
bge @8005E860
cmpwi r0, 0x8a6
beq @8005EEC0
blt @8005FFD4
cmpwi r0, 0xbf1
bge @8005FE28
b @8005FFD4
@8005E860
cmpwi r0, 0xbf5
beq @8005FE40
bge @8005FE4C
b @8005FE34
@8005E870
cmpwi r0, 0xbfe
beq @8005FE7C
bge @8005E890
cmpwi r0, 0xbfc
bge @8005FE70
cmpwi r0, 0xbfa
bge @8005FE64
b @8005FE58
@8005E890
cmpwi r0, 0xc00
beq @8005FFBC
bge @8005EEC8
b @8005FFA8
@8005E8A0
cmpwi r0, 0xc0f
beq @8005EF68
bge @8005E8D8
cmpwi r0, 0xc06
beq @8005EF2C
bge @8005E8C8
cmpwi r0, 0xc04
beq @8005EF04
bge @8005EF18
b @8005EEF0
@8005E8C8
cmpwi r0, 0xc0d
beq @8005EF40
bge @8005EF54
b @8005FFD4
@8005E8D8
cmpwi r0, 0xc13
beq @8005EFB8
bge @8005E8F4
cmpwi r0, 0xc11
beq @8005EF90
bge @8005EFA4
b @8005EF7C
@8005E8F4
cmpwi r0, 0xc15
bge @8005EFE0
b @8005EFCC
@8005E900
cmpwi r0, 0xc25
beq @8005FF68
bge @8005E96C
cmpwi r0, 0xc1e
beq @8005F094
bge @8005E944
cmpwi r0, 0xc1a
beq @8005F044
bge @8005E934
cmpwi r0, 0xc18
beq @8005F01C
bge @8005F030
b @8005F008
@8005E934
cmpwi r0, 0xc1c
beq @8005F06C
bge @8005F080
b @8005F058
@8005E944
cmpwi r0, 0xc22
beq @8005F0F8
bge @8005E960
cmpwi r0, 0xc20
beq @8005F0D0
bge @8005F0E4
b @8005F0BC
@8005E960
cmpwi r0, 0xc24
bge @8005FF28
b @8005F10C
@8005E96C
cmpwi r0, 0xc2c
beq @8005F184
bge @8005E9A0
cmpwi r0, 0xc29
beq @8005F148
bge @8005E994
cmpwi r0, 0xc27
beq @8005F120
bge @8005F134
b @8005FEE8
@8005E994
cmpwi r0, 0xc2b
bge @8005F170
b @8005F15C
@8005E9A0
cmpwi r0, 0xc35
beq @8005F1C0
bge @8005E9BC
cmpwi r0, 0xc33
beq @8005F198
bge @8005F1AC
b @8005FFD4
@8005E9BC
cmpwi r0, 0xc37
beq @8005F1E8
bge @8005F1FC
b @8005F1D4
@8005E9CC
cmpwi r0, 0xc5c
beq @8005F42C
bge @8005EAA4
cmpwi r0, 0xc48
beq @8005F350
bge @8005EA44
cmpwi r0, 0xc41
beq @8005F2B0
bge @8005EA1C
cmpwi r0, 0xc3d
beq @8005F260
bge @8005EA0C
cmpwi r0, 0xc3b
beq @8005F238
bge @8005F24C
b @8005F224
@8005EA0C
cmpwi r0, 0xc3f
beq @8005F288
bge @8005F29C
b @8005F274
@8005EA1C
cmpwi r0, 0xc45
beq @8005F314
bge @8005EA38
cmpwi r0, 0xc43
beq @8005F2D8
bge @8005F2EC
b @8005F2C4
@8005EA38
cmpwi r0, 0xc47
bge @8005F33C
b @8005F328
@8005EA44
cmpwi r0, 0xc50
beq @8005F3B4
bge @8005EA7C
cmpwi r0, 0xc4c
beq @8005FEF8
bge @8005EA6C
cmpwi r0, 0xc4a
beq @8005FF38
bge @8005FF78
b @8005F364
@8005EA6C
cmpwi r0, 0xc4e
beq @8005F38C
bge @8005F3A0
b @8005F378
@8005EA7C
cmpwi r0, 0xc59
beq @8005F3F0
bge @8005EA98
cmpwi r0, 0xc52
beq @8005F3DC
bge @8005FFD4
b @8005F3C8
@8005EA98
cmpwi r0, 0xc5b
bge @8005F418
b @8005F404
@8005EAA4
cmpwi r0, 0xc6b
beq @8005F56C
bge @8005EB10
cmpwi r0, 0xc64
beq @8005F4CC
bge @8005EAE8
cmpwi r0, 0xc60
beq @8005F47C
bge @8005EAD8
cmpwi r0, 0xc5e
beq @8005F454
bge @8005F468
b @8005F440
@8005EAD8
cmpwi r0, 0xc62
beq @8005F4A4
bge @8005F4B8
b @8005F490
@8005EAE8
cmpwi r0, 0xc68
beq @8005F51C
bge @8005EB04
cmpwi r0, 0xc66
beq @8005F4F4
bge @8005F508
b @8005F4E0
@8005EB04
cmpwi r0, 0xc6a
bge @8005F544
b @8005F530
@8005EB10
cmpwi r0, 0xc73
beq @8005F5D0
bge @8005EB48
cmpwi r0, 0xc6f
beq @8005F5BC
bge @8005EB38
cmpwi r0, 0xc6d
beq @8005F594
bge @8005F5A8
b @8005F580
@8005EB38
cmpwi r0, 0xc71
beq @8005FF88
bge @8005FF08
b @8005FF48
@8005EB48
cmpwi r0, 0xc77
beq @8005F620
bge @8005EB64
cmpwi r0, 0xc75
beq @8005F5F8
bge @8005F60C
b @8005F5E4
@8005EB64
cmpwi r0, 0xc79
bge @8005FFD4
b @8005F634
@8005EB70
cmpwi r0, 0xdd2
beq @8005FED8
bge @8005ED20
cmpwi r0, 0xdb4
beq @8005F300
bge @8005EC54
cmpwi r0, 0xc8e
beq @8005F774
bge @8005EBF4
cmpwi r0, 0xc87
beq @8005F6E8
bge @8005EBCC
cmpwi r0, 0xc83
beq @8005F698
bge @8005EBBC
cmpwi r0, 0xc81
beq @8005F670
bge @8005F684
b @8005F65C
@8005EBBC
cmpwi r0, 0xc85
beq @8005F6C0
bge @8005F6D4
b @8005F6AC
@8005EBCC
cmpwi r0, 0xc8b
beq @8005F738
bge @8005EBE8
cmpwi r0, 0xc89
beq @8005F710
bge @8005F724
b @8005F6FC
@8005EBE8
cmpwi r0, 0xc8d
bge @8005F760
b @8005F74C
@8005EBF4
cmpwi r0, 0xc96
beq @8005FF58
bge @8005EC2C
cmpwi r0, 0xc92
beq @8005F7D8
bge @8005EC1C
cmpwi r0, 0xc90
beq @8005F79C
bge @8005F7C4
b @8005F788
@8005EC1C
cmpwi r0, 0xc94
beq @8005F800
bge @8005F814
b @8005F7EC
@8005EC2C
cmpwi r0, 0xdaf
beq @8005F558
bge @8005EC48
cmpwi r0, 0xc98
beq @8005FF18
bge @8005FFD4
b @8005FF98
@8005EC48
cmpwi r0, 0xdb3
bge @8005F7B0
b @8005FFD4
@8005EC54
cmpwi r0, 0xdc3
beq @8005F92C
bge @8005ECC0
cmpwi r0, 0xdbc
beq @8005F8A0
bge @8005EC98
cmpwi r0, 0xdb8
beq @8005F850
bge @8005EC88
cmpwi r0, 0xdb6
beq @8005F828
bge @8005F83C
b @8005F0A8
@8005EC88
cmpwi r0, 0xdba
beq @8005F878
bge @8005F88C
b @8005F864
@8005EC98
cmpwi r0, 0xdc0
beq @8005F8F0
bge @8005ECB4
cmpwi r0, 0xdbe
beq @8005F8C8
bge @8005F8DC
b @8005F8B4
@8005ECB4
cmpwi r0, 0xdc2
bge @8005F918
b @8005F904
@8005ECC0
cmpwi r0, 0xdcb
beq @8005F9CC
bge @8005ECF8
cmpwi r0, 0xdc7
beq @8005F97C
bge @8005ECE8
cmpwi r0, 0xdc5
beq @8005F954
bge @8005F968
b @8005F940
@8005ECE8
cmpwi r0, 0xdc9
beq @8005F9A4
bge @8005F9B8
b @8005F990
@8005ECF8
cmpwi r0, 0xdcf
beq @8005FEA8
bge @8005ED14
cmpwi r0, 0xdcd
beq @8005F9F4
bge @8005FEB8
b @8005F9E0
@8005ED14
cmpwi r0, 0xdd1
bge @8005FE98
b @8005FE88
@8005ED20
cmpwi r0, 0xdf0
beq @8005FCD8
bge @8005EDF8
cmpwi r0, 0xde1
beq @8005FB70
bge @8005ED98
cmpwi r0, 0xdda
beq @8005FA30
bge @8005ED70
cmpwi r0, 0xdd6
beq @8005FDE8
bge @8005ED60
cmpwi r0, 0xdd4
beq @8005FDC8
bge @8005FDD8
b @8005FEC8
@8005ED60
cmpwi r0, 0xdd8
beq @8005FA08
bge @8005FA1C
b @8005FDF8
@8005ED70
cmpwi r0, 0xdde
beq @8005FAF8
bge @8005ED8C
cmpwi r0, 0xddc
beq @8005FA94
bge @8005FAA8
b @8005FA80
@8005ED8C
cmpwi r0, 0xde0
bge @8005FB20
b @8005FB0C
@8005ED98
cmpwi r0, 0xde9
beq @8005FAE4
bge @8005EDD0
cmpwi r0, 0xde5
beq @8005FA58
bge @8005EDC0
cmpwi r0, 0xde3
beq @8005FB98
bge @8005FA44
b @8005FB84
@8005EDC0
cmpwi r0, 0xde7
beq @8005FABC
bge @8005FAD0
b @8005FA6C
@8005EDD0
cmpwi r0, 0xded
beq @8005FBAC
bge @8005EDEC
cmpwi r0, 0xdeb
beq @8005FB48
bge @8005FB5C
b @8005FB34
@8005EDEC
cmpwi r0, 0xdef
bge @8005FBD4
b @8005FBC0
@8005EDF8
cmpwi r0, 0xdff
beq @8005FC24
bge @8005EE64
cmpwi r0, 0xdf8
beq @8005FC88
bge @8005EE3C
cmpwi r0, 0xdf4
beq @8005FBFC
bge @8005EE2C
cmpwi r0, 0xdf2
beq @8005FD00
bge @8005FBE8
b @8005FCEC
@8005EE2C
cmpwi r0, 0xdf6
beq @8005FC60
bge @8005FC74
b @8005FC10
@8005EE3C
cmpwi r0, 0xdfc
beq @8005FD14
bge @8005EE58
cmpwi r0, 0xdfa
beq @8005FD64
bge @8005FD78
b @8005FD50
@8005EE58
cmpwi r0, 0xdfe
bge @8005FD3C
b @8005FD28
@8005EE64
cmpwi r0, 0xe07
beq @8005FDB4
bge @8005EE9C
cmpwi r0, 0xe03
beq @8005FCB0
bge @8005EE8C
cmpwi r0, 0xe01
beq @8005FC4C
bge @8005FC9C
b @8005FC38
@8005EE8C
cmpwi r0, 0xe05
beq @8005FD8C
bge @8005FDA0
b @8005FCC4
@8005EE9C
cmpwi r0, 0x102d
beq @8005FE18
bge @8005EEB4
cmpwi r0, 0x102c
bge @8005FE08
b @8005FFD4
@8005EEB4
cmpwi r0, 0x1096
beq @8005FFD0
b @8005FFD4
@8005EEC0
bl fn_800608C4
b @8005FFD4
@8005EEC8
lwz r6, 0x0(r5)
li r5, 0x0
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005EEDC
lwz r6, 0x4(r5)
li r5, 0x0
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005EEF0
lwz r6, 0x8(r5)
li r5, 0x0
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005EF04
lwz r6, 0xc(r5)
li r5, 0x0
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005EF18
lwz r6, 0x10(r5)
li r5, 0x0
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005EF2C
lwz r6, 0x14(r5)
li r5, 0x0
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005EF40
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005EF54
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005EF68
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005EF7C
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005EF90
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005EFA4
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005EFB8
lwz r6, 0x0(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005EFCC
lwz r6, 0x4(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005EFE0
lwz r6, 0x8(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005EFF4
lwz r6, 0xc(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F008
lwz r6, 0x10(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F01C
lwz r6, 0x14(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F030
lwz r6, 0x0(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F044
lwz r6, 0x4(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F058
lwz r6, 0x8(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F06C
lwz r6, 0xc(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F080
lwz r6, 0x10(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F094
lwz r6, 0x14(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F0A8
lwz r6, 0x0(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F0BC
lwz r6, 0x4(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F0D0
lwz r6, 0x8(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F0E4
lwz r6, 0xc(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F0F8
lwz r6, 0x10(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F10C
lwz r6, 0x14(r5)
li r5, 0x0
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F120
lwz r6, 0x18(r5)
li r5, 0x1
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F134
lwz r6, 0x1c(r5)
li r5, 0x1
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F148
lwz r6, 0x20(r5)
li r5, 0x1
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F15C
lwz r6, 0x24(r5)
li r5, 0x1
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F170
lwz r6, 0x28(r5)
li r5, 0x1
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F184
lwz r6, 0x2c(r5)
li r5, 0x1
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F198
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F1AC
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F1C0
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F1D4
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F1E8
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F1FC
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F210
lwz r6, 0x18(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F224
lwz r6, 0x1c(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F238
lwz r6, 0x20(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F24C
lwz r6, 0x24(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F260
lwz r6, 0x28(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F274
lwz r6, 0x2c(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F288
lwz r6, 0x18(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F29C
lwz r6, 0x1c(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F2B0
lwz r6, 0x20(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F2C4
lwz r6, 0x24(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F2D8
lwz r6, 0x28(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F2EC
lwz r6, 0x2c(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F300
lwz r6, 0x18(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F314
lwz r6, 0x1c(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F328
lwz r6, 0x20(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F33C
lwz r6, 0x24(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F350
lwz r6, 0x28(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F364
lwz r6, 0x2c(r5)
li r5, 0x1
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F378
lwz r6, 0x30(r5)
li r5, 0x2
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F38C
lwz r6, 0x34(r5)
li r5, 0x2
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F3A0
lwz r6, 0x38(r5)
li r5, 0x2
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F3B4
lwz r6, 0x3c(r5)
li r5, 0x2
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F3C8
lwz r6, 0x40(r5)
li r5, 0x2
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F3DC
lwz r6, 0x44(r5)
li r5, 0x2
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F3F0
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F404
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F418
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F42C
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F440
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F454
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F468
lwz r6, 0x30(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F47C
lwz r6, 0x34(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F490
lwz r6, 0x38(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F4A4
lwz r6, 0x3c(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F4B8
lwz r6, 0x40(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F4CC
lwz r6, 0x44(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F4E0
lwz r6, 0x30(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F4F4
lwz r6, 0x34(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F508
lwz r6, 0x38(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F51C
lwz r6, 0x3c(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F530
lwz r6, 0x40(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F544
lwz r6, 0x44(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F558
lwz r6, 0x30(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F56C
lwz r6, 0x34(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F580
lwz r6, 0x38(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F594
lwz r6, 0x3c(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F5A8
lwz r6, 0x40(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F5BC
lwz r6, 0x44(r5)
li r5, 0x2
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F5D0
lwz r6, 0x48(r5)
li r5, 0x3
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F5E4
lwz r6, 0x4c(r5)
li r5, 0x3
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F5F8
lwz r6, 0x50(r5)
li r5, 0x3
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F60C
lwz r6, 0x54(r5)
li r5, 0x3
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F620
lwz r6, 0x58(r5)
li r5, 0x3
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F634
lwz r6, 0x5c(r5)
li r5, 0x3
li r7, 0x2
bl fn_8006106C
b @8005FFD4
@8005F648
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F65C
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F670
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F684
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F698
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F6AC
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F6C0
lwz r6, 0x48(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F6D4
lwz r6, 0x4c(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F6E8
lwz r6, 0x50(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F6FC
lwz r6, 0x54(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F710
lwz r6, 0x58(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F724
lwz r6, 0x5c(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061A2C
b @8005FFD4
@8005F738
lwz r6, 0x48(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F74C
lwz r6, 0x4c(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F760
lwz r6, 0x50(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F774
lwz r6, 0x54(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F788
lwz r6, 0x58(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F79C
lwz r6, 0x5c(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061BBC
b @8005FFD4
@8005F7B0
lwz r6, 0x48(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F7C4
lwz r6, 0x4c(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F7D8
lwz r6, 0x50(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F7EC
lwz r6, 0x54(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F800
lwz r6, 0x58(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F814
lwz r6, 0x5c(r5)
li r5, 0x3
li r7, 0x2
bl fn_80061B74
b @8005FFD4
@8005F828
lwz r6, 0x0(r5)
li r5, 0x0
li r7, 0x0
bl fn_8006106C
b @8005FFD4
@8005F83C
lwz r6, 0x4(r5)
li r5, 0x0
li r7, 0x0
bl fn_8006106C
b @8005FFD4
@8005F850
lwz r6, 0x8(r5)
li r5, 0x0
li r7, 0x0
bl fn_8006106C
b @8005FFD4
@8005F864
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F878
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F88C
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F8A0
lwz r6, 0x0(r5)
li r5, 0x0
li r7, 0x0
bl fn_80061A2C
b @8005FFD4
@8005F8B4
lwz r6, 0x4(r5)
li r5, 0x0
li r7, 0x0
bl fn_80061A2C
b @8005FFD4
@8005F8C8
lwz r6, 0x8(r5)
li r5, 0x0
li r7, 0x0
bl fn_80061A2C
b @8005FFD4
@8005F8DC
lwz r6, 0x0(r5)
li r5, 0x0
li r7, 0x0
bl fn_80061BBC
b @8005FFD4
@8005F8F0
lwz r6, 0x4(r5)
li r5, 0x0
li r7, 0x0
bl fn_80061BBC
b @8005FFD4
@8005F904
lwz r6, 0x8(r5)
li r5, 0x0
li r7, 0x0
bl fn_80061BBC
b @8005FFD4
@8005F918
lwz r6, 0x18(r5)
li r5, 0x1
li r7, 0x0
bl fn_8006106C
b @8005FFD4
@8005F92C
lwz r6, 0x1c(r5)
li r5, 0x1
li r7, 0x0
bl fn_8006106C
b @8005FFD4
@8005F940
lwz r6, 0x20(r5)
li r5, 0x1
li r7, 0x0
bl fn_8006106C
b @8005FFD4
@8005F954
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F968
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F97C
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005F990
lwz r6, 0x18(r5)
li r5, 0x1
li r7, 0x0
bl fn_80061A2C
b @8005FFD4
@8005F9A4
lwz r6, 0x1c(r5)
li r5, 0x1
li r7, 0x0
bl fn_80061A2C
b @8005FFD4
@8005F9B8
lwz r6, 0x20(r5)
li r5, 0x1
li r7, 0x0
bl fn_80061A2C
b @8005FFD4
@8005F9CC
lwz r6, 0x18(r5)
li r5, 0x1
li r7, 0x0
bl fn_80061BBC
b @8005FFD4
@8005F9E0
lwz r6, 0x1c(r5)
li r5, 0x1
li r7, 0x0
bl fn_80061BBC
b @8005FFD4
@8005F9F4
lwz r6, 0x20(r5)
li r5, 0x1
li r7, 0x0
bl fn_80061BBC
b @8005FFD4
@8005FA08
lwz r6, 0x0(r5)
li r5, 0x0
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FA1C
lwz r6, 0x4(r5)
li r5, 0x0
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FA30
lwz r6, 0x8(r5)
li r5, 0x0
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FA44
lwz r6, 0xc(r5)
li r5, 0x0
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FA58
lwz r6, 0x10(r5)
li r5, 0x0
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FA6C
lwz r6, 0x14(r5)
li r5, 0x0
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FA80
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FA94
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FAA8
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FABC
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FAD0
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FAE4
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FAF8
lwz r6, 0x0(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FB0C
lwz r6, 0x4(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FB20
lwz r6, 0x8(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FB34
lwz r6, 0xc(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FB48
lwz r6, 0x10(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FB5C
lwz r6, 0x14(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FB70
lwz r6, 0x0(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FB84
lwz r6, 0x4(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FB98
lwz r6, 0x8(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FBAC
lwz r6, 0xc(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FBC0
lwz r6, 0x10(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FBD4
lwz r6, 0x14(r5)
li r5, 0x0
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FBE8
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FBFC
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FC10
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FC24
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FC38
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FC4C
lbz r0, 0x4(r4)
rlwinm r0, r0, 0, 31, 29
extsb r0, r0
stb r0, 0x4(r4)
b @8005FFD4
@8005FC60
lwz r6, 0x18(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FC74
lwz r6, 0x1c(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FC88
lwz r6, 0x20(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FC9C
lwz r6, 0x24(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FCB0
lwz r6, 0x28(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FCC4
lwz r6, 0x2c(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061A2C
b @8005FFD4
@8005FCD8
lwz r6, 0x18(r5)
li r5, 0x1
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FCEC
lwz r6, 0x1c(r5)
li r5, 0x1
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FD00
lwz r6, 0x20(r5)
li r5, 0x1
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FD14
lwz r6, 0x24(r5)
li r5, 0x1
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FD28
lwz r6, 0x28(r5)
li r5, 0x1
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FD3C
lwz r6, 0x2c(r5)
li r5, 0x1
li r7, 0x1
bl fn_8006106C
b @8005FFD4
@8005FD50
lwz r6, 0x18(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FD64
lwz r6, 0x1c(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FD78
lwz r6, 0x20(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FD8C
lwz r6, 0x24(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FDA0
lwz r6, 0x28(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FDB4
lwz r6, 0x2c(r5)
li r5, 0x1
li r7, 0x1
bl fn_80061BBC
b @8005FFD4
@8005FDC8
li r5, 0x0
li r6, 0x0
bl fn_80060D70
b @8005FFD4
@8005FDD8
li r5, 0x0
li r6, 0x1
bl fn_80060D70
b @8005FFD4
@8005FDE8
li r5, 0x1
li r6, 0x0
bl fn_80060D70
b @8005FFD4
@8005FDF8
li r5, 0x1
li r6, 0x1
bl fn_80060D70
b @8005FFD4
@8005FE08
li r5, 0x1
li r6, 0x2
bl fn_80060D70
b @8005FFD4
@8005FE18
li r5, 0x1
li r6, 0x2
bl fn_80060D70
b @8005FFD4
@8005FE28
li r5, 0x6
bl fn_80060EF4
b @8005FFD4
@8005FE34
li r5, 0x6
bl fn_80060EF4
b @8005FFD4
@8005FE40
li r5, -0x1
bl fn_80060EF4
b @8005FFD4
@8005FE4C
li r5, 0x3
bl fn_80060EF4
b @8005FFD4
@8005FE58
li r5, 0x4
bl fn_80060EF4
b @8005FFD4
@8005FE64
li r5, 0x2
bl fn_80060EF4
b @8005FFD4
@8005FE70
li r5, 0x1
bl fn_80060EF4
b @8005FFD4
@8005FE7C
li r5, 0x0
bl fn_80060EF4
b @8005FFD4
@8005FE88
li r5, 0x0
li r6, 0x0
bl fn_800617E0
b @8005FFD4
@8005FE98
li r5, 0x1
li r6, 0x0
bl fn_800617E0
b @8005FFD4
@8005FEA8
li r5, 0x0
li r6, 0x0
bl fn_800615F4
b @8005FFD4
@8005FEB8
li r5, 0x1
li r6, 0x0
bl fn_800615F4
b @8005FFD4
@8005FEC8
li r5, 0x0
li r6, 0x0
bl fn_80061454
b @8005FFD4
@8005FED8
li r5, 0x1
li r6, 0x0
bl fn_80061454
b @8005FFD4
@8005FEE8
li r5, 0x0
li r6, 0x2
bl fn_800617E0
b @8005FFD4
@8005FEF8
li r5, 0x1
li r6, 0x2
bl fn_800617E0
b @8005FFD4
@8005FF08
li r5, 0x2
li r6, 0x2
bl fn_800617E0
b @8005FFD4
@8005FF18
li r5, 0x3
li r6, 0x2
bl fn_800617E0
b @8005FFD4
@8005FF28
li r5, 0x0
li r6, 0x2
bl fn_80061454
b @8005FFD4
@8005FF38
li r5, 0x1
li r6, 0x2
bl fn_80061454
b @8005FFD4
@8005FF48
li r5, 0x2
li r6, 0x2
bl fn_80061454
b @8005FFD4
@8005FF58
li r5, 0x3
li r6, 0x2
bl fn_80061454
b @8005FFD4
@8005FF68
li r5, 0x0
li r6, 0x2
bl fn_800615F4
b @8005FFD4
@8005FF78
li r5, 0x1
li r6, 0x2
bl fn_800615F4
b @8005FFD4
@8005FF88
li r5, 0x2
li r6, 0x2
bl fn_800615F4
b @8005FFD4
@8005FF98
li r5, 0x3
li r6, 0x2
bl fn_800615F4
b @8005FFD4
@8005FFA8
lis r5, lbl_803A9A60@ha
addi r5, r5, lbl_803A9A60@l
lfs f1, 0x48(r5)
bl fn_800609B4
b @8005FFD4
@8005FFBC
lis r5, lbl_803A9A60@ha
addi r5, r5, lbl_803A9A60@l
lfs f1, 0x4c(r5)
bl fn_800609B4
b @8005FFD4
@8005FFD0
bl fn_80060434
@8005FFD4
lwz r0, 0x14(r1)
mtlr r0
addi r1, r1, 0x10
blr
