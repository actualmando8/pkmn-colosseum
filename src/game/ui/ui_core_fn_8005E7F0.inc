    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lha r0, 0x6(r4)
    lis r5, lbl_803A9E40@ha
    addi r5, r5, lbl_803A9E40@l
    cmpwi r0, 0xc7f
    beq @L_8005F648
    bge @L_8005EB70
    cmpwi r0, 0xc39
    beq @L_8005F210
    bge @L_8005E9CC
    cmpwi r0, 0xc16
    beq @L_8005EFF4
    bge @L_8005E900
    cmpwi r0, 0xc02
    beq @L_8005EEDC
    bge @L_8005E8A0
    cmpwi r0, 0xbf8
    bge @L_8005E870
    cmpwi r0, 0xbf3
    bge @L_8005E860
    cmpwi r0, 0x8a6
    beq @L_8005EEC0
    blt @L_8005FFD4
    cmpwi r0, 0xbf1
    bge @L_8005FE28
    b @L_8005FFD4
@L_8005E860:
    cmpwi r0, 0xbf5
    beq @L_8005FE40
    bge @L_8005FE4C
    b @L_8005FE34
@L_8005E870:
    cmpwi r0, 0xbfe
    beq @L_8005FE7C
    bge @L_8005E890
    cmpwi r0, 0xbfc
    bge @L_8005FE70
    cmpwi r0, 0xbfa
    bge @L_8005FE64
    b @L_8005FE58
@L_8005E890:
    cmpwi r0, 0xc00
    beq @L_8005FFBC
    bge @L_8005EEC8
    b @L_8005FFA8
@L_8005E8A0:
    cmpwi r0, 0xc0f
    beq @L_8005EF68
    bge @L_8005E8D8
    cmpwi r0, 0xc06
    beq @L_8005EF2C
    bge @L_8005E8C8
    cmpwi r0, 0xc04
    beq @L_8005EF04
    bge @L_8005EF18
    b @L_8005EEF0
@L_8005E8C8:
    cmpwi r0, 0xc0d
    beq @L_8005EF40
    bge @L_8005EF54
    b @L_8005FFD4
@L_8005E8D8:
    cmpwi r0, 0xc13
    beq @L_8005EFB8
    bge @L_8005E8F4
    cmpwi r0, 0xc11
    beq @L_8005EF90
    bge @L_8005EFA4
    b @L_8005EF7C
@L_8005E8F4:
    cmpwi r0, 0xc15
    bge @L_8005EFE0
    b @L_8005EFCC
@L_8005E900:
    cmpwi r0, 0xc25
    beq @L_8005FF68
    bge @L_8005E96C
    cmpwi r0, 0xc1e
    beq @L_8005F094
    bge @L_8005E944
    cmpwi r0, 0xc1a
    beq @L_8005F044
    bge @L_8005E934
    cmpwi r0, 0xc18
    beq @L_8005F01C
    bge @L_8005F030
    b @L_8005F008
@L_8005E934:
    cmpwi r0, 0xc1c
    beq @L_8005F06C
    bge @L_8005F080
    b @L_8005F058
@L_8005E944:
    cmpwi r0, 0xc22
    beq @L_8005F0F8
    bge @L_8005E960
    cmpwi r0, 0xc20
    beq @L_8005F0D0
    bge @L_8005F0E4
    b @L_8005F0BC
@L_8005E960:
    cmpwi r0, 0xc24
    bge @L_8005FF28
    b @L_8005F10C
@L_8005E96C:
    cmpwi r0, 0xc2c
    beq @L_8005F184
    bge @L_8005E9A0
    cmpwi r0, 0xc29
    beq @L_8005F148
    bge @L_8005E994
    cmpwi r0, 0xc27
    beq @L_8005F120
    bge @L_8005F134
    b @L_8005FEE8
@L_8005E994:
    cmpwi r0, 0xc2b
    bge @L_8005F170
    b @L_8005F15C
@L_8005E9A0:
    cmpwi r0, 0xc35
    beq @L_8005F1C0
    bge @L_8005E9BC
    cmpwi r0, 0xc33
    beq @L_8005F198
    bge @L_8005F1AC
    b @L_8005FFD4
@L_8005E9BC:
    cmpwi r0, 0xc37
    beq @L_8005F1E8
    bge @L_8005F1FC
    b @L_8005F1D4
@L_8005E9CC:
    cmpwi r0, 0xc5c
    beq @L_8005F42C
    bge @L_8005EAA4
    cmpwi r0, 0xc48
    beq @L_8005F350
    bge @L_8005EA44
    cmpwi r0, 0xc41
    beq @L_8005F2B0
    bge @L_8005EA1C
    cmpwi r0, 0xc3d
    beq @L_8005F260
    bge @L_8005EA0C
    cmpwi r0, 0xc3b
    beq @L_8005F238
    bge @L_8005F24C
    b @L_8005F224
@L_8005EA0C:
    cmpwi r0, 0xc3f
    beq @L_8005F288
    bge @L_8005F29C
    b @L_8005F274
@L_8005EA1C:
    cmpwi r0, 0xc45
    beq @L_8005F314
    bge @L_8005EA38
    cmpwi r0, 0xc43
    beq @L_8005F2D8
    bge @L_8005F2EC
    b @L_8005F2C4
@L_8005EA38:
    cmpwi r0, 0xc47
    bge @L_8005F33C
    b @L_8005F328
@L_8005EA44:
    cmpwi r0, 0xc50
    beq @L_8005F3B4
    bge @L_8005EA7C
    cmpwi r0, 0xc4c
    beq @L_8005FEF8
    bge @L_8005EA6C
    cmpwi r0, 0xc4a
    beq @L_8005FF38
    bge @L_8005FF78
    b @L_8005F364
@L_8005EA6C:
    cmpwi r0, 0xc4e
    beq @L_8005F38C
    bge @L_8005F3A0
    b @L_8005F378
@L_8005EA7C:
    cmpwi r0, 0xc59
    beq @L_8005F3F0
    bge @L_8005EA98
    cmpwi r0, 0xc52
    beq @L_8005F3DC
    bge @L_8005FFD4
    b @L_8005F3C8
@L_8005EA98:
    cmpwi r0, 0xc5b
    bge @L_8005F418
    b @L_8005F404
@L_8005EAA4:
    cmpwi r0, 0xc6b
    beq @L_8005F56C
    bge @L_8005EB10
    cmpwi r0, 0xc64
    beq @L_8005F4CC
    bge @L_8005EAE8
    cmpwi r0, 0xc60
    beq @L_8005F47C
    bge @L_8005EAD8
    cmpwi r0, 0xc5e
    beq @L_8005F454
    bge @L_8005F468
    b @L_8005F440
@L_8005EAD8:
    cmpwi r0, 0xc62
    beq @L_8005F4A4
    bge @L_8005F4B8
    b @L_8005F490
@L_8005EAE8:
    cmpwi r0, 0xc68
    beq @L_8005F51C
    bge @L_8005EB04
    cmpwi r0, 0xc66
    beq @L_8005F4F4
    bge @L_8005F508
    b @L_8005F4E0
@L_8005EB04:
    cmpwi r0, 0xc6a
    bge @L_8005F544
    b @L_8005F530
@L_8005EB10:
    cmpwi r0, 0xc73
    beq @L_8005F5D0
    bge @L_8005EB48
    cmpwi r0, 0xc6f
    beq @L_8005F5BC
    bge @L_8005EB38
    cmpwi r0, 0xc6d
    beq @L_8005F594
    bge @L_8005F5A8
    b @L_8005F580
@L_8005EB38:
    cmpwi r0, 0xc71
    beq @L_8005FF88
    bge @L_8005FF08
    b @L_8005FF48
@L_8005EB48:
    cmpwi r0, 0xc77
    beq @L_8005F620
    bge @L_8005EB64
    cmpwi r0, 0xc75
    beq @L_8005F5F8
    bge @L_8005F60C
    b @L_8005F5E4
@L_8005EB64:
    cmpwi r0, 0xc79
    bge @L_8005FFD4
    b @L_8005F634
@L_8005EB70:
    cmpwi r0, 0xdd2
    beq @L_8005FED8
    bge @L_8005ED20
    cmpwi r0, 0xdb4
    beq @L_8005F300
    bge @L_8005EC54
    cmpwi r0, 0xc8e
    beq @L_8005F774
    bge @L_8005EBF4
    cmpwi r0, 0xc87
    beq @L_8005F6E8
    bge @L_8005EBCC
    cmpwi r0, 0xc83
    beq @L_8005F698
    bge @L_8005EBBC
    cmpwi r0, 0xc81
    beq @L_8005F670
    bge @L_8005F684
    b @L_8005F65C
@L_8005EBBC:
    cmpwi r0, 0xc85
    beq @L_8005F6C0
    bge @L_8005F6D4
    b @L_8005F6AC
@L_8005EBCC:
    cmpwi r0, 0xc8b
    beq @L_8005F738
    bge @L_8005EBE8
    cmpwi r0, 0xc89
    beq @L_8005F710
    bge @L_8005F724
    b @L_8005F6FC
@L_8005EBE8:
    cmpwi r0, 0xc8d
    bge @L_8005F760
    b @L_8005F74C
@L_8005EBF4:
    cmpwi r0, 0xc96
    beq @L_8005FF58
    bge @L_8005EC2C
    cmpwi r0, 0xc92
    beq @L_8005F7D8
    bge @L_8005EC1C
    cmpwi r0, 0xc90
    beq @L_8005F79C
    bge @L_8005F7C4
    b @L_8005F788
@L_8005EC1C:
    cmpwi r0, 0xc94
    beq @L_8005F800
    bge @L_8005F814
    b @L_8005F7EC
@L_8005EC2C:
    cmpwi r0, 0xdaf
    beq @L_8005F558
    bge @L_8005EC48
    cmpwi r0, 0xc98
    beq @L_8005FF18
    bge @L_8005FFD4
    b @L_8005FF98
@L_8005EC48:
    cmpwi r0, 0xdb3
    bge @L_8005F7B0
    b @L_8005FFD4
@L_8005EC54:
    cmpwi r0, 0xdc3
    beq @L_8005F92C
    bge @L_8005ECC0
    cmpwi r0, 0xdbc
    beq @L_8005F8A0
    bge @L_8005EC98
    cmpwi r0, 0xdb8
    beq @L_8005F850
    bge @L_8005EC88
    cmpwi r0, 0xdb6
    beq @L_8005F828
    bge @L_8005F83C
    b @L_8005F0A8
@L_8005EC88:
    cmpwi r0, 0xdba
    beq @L_8005F878
    bge @L_8005F88C
    b @L_8005F864
@L_8005EC98:
    cmpwi r0, 0xdc0
    beq @L_8005F8F0
    bge @L_8005ECB4
    cmpwi r0, 0xdbe
    beq @L_8005F8C8
    bge @L_8005F8DC
    b @L_8005F8B4
@L_8005ECB4:
    cmpwi r0, 0xdc2
    bge @L_8005F918
    b @L_8005F904
@L_8005ECC0:
    cmpwi r0, 0xdcb
    beq @L_8005F9CC
    bge @L_8005ECF8
    cmpwi r0, 0xdc7
    beq @L_8005F97C
    bge @L_8005ECE8
    cmpwi r0, 0xdc5
    beq @L_8005F954
    bge @L_8005F968
    b @L_8005F940
@L_8005ECE8:
    cmpwi r0, 0xdc9
    beq @L_8005F9A4
    bge @L_8005F9B8
    b @L_8005F990
@L_8005ECF8:
    cmpwi r0, 0xdcf
    beq @L_8005FEA8
    bge @L_8005ED14
    cmpwi r0, 0xdcd
    beq @L_8005F9F4
    bge @L_8005FEB8
    b @L_8005F9E0
@L_8005ED14:
    cmpwi r0, 0xdd1
    bge @L_8005FE98
    b @L_8005FE88
@L_8005ED20:
    cmpwi r0, 0xdf0
    beq @L_8005FCD8
    bge @L_8005EDF8
    cmpwi r0, 0xde1
    beq @L_8005FB70
    bge @L_8005ED98
    cmpwi r0, 0xdda
    beq @L_8005FA30
    bge @L_8005ED70
    cmpwi r0, 0xdd6
    beq @L_8005FDE8
    bge @L_8005ED60
    cmpwi r0, 0xdd4
    beq @L_8005FDC8
    bge @L_8005FDD8
    b @L_8005FEC8
@L_8005ED60:
    cmpwi r0, 0xdd8
    beq @L_8005FA08
    bge @L_8005FA1C
    b @L_8005FDF8
@L_8005ED70:
    cmpwi r0, 0xdde
    beq @L_8005FAF8
    bge @L_8005ED8C
    cmpwi r0, 0xddc
    beq @L_8005FA94
    bge @L_8005FAA8
    b @L_8005FA80
@L_8005ED8C:
    cmpwi r0, 0xde0
    bge @L_8005FB20
    b @L_8005FB0C
@L_8005ED98:
    cmpwi r0, 0xde9
    beq @L_8005FAE4
    bge @L_8005EDD0
    cmpwi r0, 0xde5
    beq @L_8005FA58
    bge @L_8005EDC0
    cmpwi r0, 0xde3
    beq @L_8005FB98
    bge @L_8005FA44
    b @L_8005FB84
@L_8005EDC0:
    cmpwi r0, 0xde7
    beq @L_8005FABC
    bge @L_8005FAD0
    b @L_8005FA6C
@L_8005EDD0:
    cmpwi r0, 0xded
    beq @L_8005FBAC
    bge @L_8005EDEC
    cmpwi r0, 0xdeb
    beq @L_8005FB48
    bge @L_8005FB5C
    b @L_8005FB34
@L_8005EDEC:
    cmpwi r0, 0xdef
    bge @L_8005FBD4
    b @L_8005FBC0
@L_8005EDF8:
    cmpwi r0, 0xdff
    beq @L_8005FC24
    bge @L_8005EE64
    cmpwi r0, 0xdf8
    beq @L_8005FC88
    bge @L_8005EE3C
    cmpwi r0, 0xdf4
    beq @L_8005FBFC
    bge @L_8005EE2C
    cmpwi r0, 0xdf2
    beq @L_8005FD00
    bge @L_8005FBE8
    b @L_8005FCEC
@L_8005EE2C:
    cmpwi r0, 0xdf6
    beq @L_8005FC60
    bge @L_8005FC74
    b @L_8005FC10
@L_8005EE3C:
    cmpwi r0, 0xdfc
    beq @L_8005FD14
    bge @L_8005EE58
    cmpwi r0, 0xdfa
    beq @L_8005FD64
    bge @L_8005FD78
    b @L_8005FD50
@L_8005EE58:
    cmpwi r0, 0xdfe
    bge @L_8005FD3C
    b @L_8005FD28
@L_8005EE64:
    cmpwi r0, 0xe07
    beq @L_8005FDB4
    bge @L_8005EE9C
    cmpwi r0, 0xe03
    beq @L_8005FCB0
    bge @L_8005EE8C
    cmpwi r0, 0xe01
    beq @L_8005FC4C
    bge @L_8005FC9C
    b @L_8005FC38
@L_8005EE8C:
    cmpwi r0, 0xe05
    beq @L_8005FD8C
    bge @L_8005FDA0
    b @L_8005FCC4
@L_8005EE9C:
    cmpwi r0, 0x102d
    beq @L_8005FE18
    bge @L_8005EEB4
    cmpwi r0, 0x102c
    bge @L_8005FE08
    b @L_8005FFD4
@L_8005EEB4:
    cmpwi r0, 0x1096
    beq @L_8005FFD0
    b @L_8005FFD4
@L_8005EEC0:
    bl fn_800608C4
    b @L_8005FFD4
@L_8005EEC8:
    lwz r6, 0x0(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005EEDC:
    lwz r6, 0x4(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005EEF0:
    lwz r6, 0x8(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005EF04:
    lwz r6, 0xc(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005EF18:
    lwz r6, 0x10(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005EF2C:
    lwz r6, 0x14(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005EF40:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005EF54:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005EF68:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005EF7C:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005EF90:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005EFA4:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005EFB8:
    lwz r6, 0x0(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005EFCC:
    lwz r6, 0x4(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005EFE0:
    lwz r6, 0x8(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005EFF4:
    lwz r6, 0xc(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F008:
    lwz r6, 0x10(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F01C:
    lwz r6, 0x14(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F030:
    lwz r6, 0x0(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F044:
    lwz r6, 0x4(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F058:
    lwz r6, 0x8(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F06C:
    lwz r6, 0xc(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F080:
    lwz r6, 0x10(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F094:
    lwz r6, 0x14(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F0A8:
    lwz r6, 0x0(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F0BC:
    lwz r6, 0x4(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F0D0:
    lwz r6, 0x8(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F0E4:
    lwz r6, 0xc(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F0F8:
    lwz r6, 0x10(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F10C:
    lwz r6, 0x14(r5)
    li r5, 0x0
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F120:
    lwz r6, 0x18(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F134:
    lwz r6, 0x1c(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F148:
    lwz r6, 0x20(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F15C:
    lwz r6, 0x24(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F170:
    lwz r6, 0x28(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F184:
    lwz r6, 0x2c(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F198:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F1AC:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F1C0:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F1D4:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F1E8:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F1FC:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F210:
    lwz r6, 0x18(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F224:
    lwz r6, 0x1c(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F238:
    lwz r6, 0x20(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F24C:
    lwz r6, 0x24(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F260:
    lwz r6, 0x28(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F274:
    lwz r6, 0x2c(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F288:
    lwz r6, 0x18(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F29C:
    lwz r6, 0x1c(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F2B0:
    lwz r6, 0x20(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F2C4:
    lwz r6, 0x24(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F2D8:
    lwz r6, 0x28(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F2EC:
    lwz r6, 0x2c(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F300:
    lwz r6, 0x18(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F314:
    lwz r6, 0x1c(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F328:
    lwz r6, 0x20(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F33C:
    lwz r6, 0x24(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F350:
    lwz r6, 0x28(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F364:
    lwz r6, 0x2c(r5)
    li r5, 0x1
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F378:
    lwz r6, 0x30(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F38C:
    lwz r6, 0x34(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F3A0:
    lwz r6, 0x38(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F3B4:
    lwz r6, 0x3c(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F3C8:
    lwz r6, 0x40(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F3DC:
    lwz r6, 0x44(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F3F0:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F404:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F418:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F42C:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F440:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F454:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F468:
    lwz r6, 0x30(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F47C:
    lwz r6, 0x34(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F490:
    lwz r6, 0x38(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F4A4:
    lwz r6, 0x3c(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F4B8:
    lwz r6, 0x40(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F4CC:
    lwz r6, 0x44(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F4E0:
    lwz r6, 0x30(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F4F4:
    lwz r6, 0x34(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F508:
    lwz r6, 0x38(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F51C:
    lwz r6, 0x3c(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F530:
    lwz r6, 0x40(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F544:
    lwz r6, 0x44(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F558:
    lwz r6, 0x30(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F56C:
    lwz r6, 0x34(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F580:
    lwz r6, 0x38(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F594:
    lwz r6, 0x3c(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F5A8:
    lwz r6, 0x40(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F5BC:
    lwz r6, 0x44(r5)
    li r5, 0x2
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F5D0:
    lwz r6, 0x48(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F5E4:
    lwz r6, 0x4c(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F5F8:
    lwz r6, 0x50(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F60C:
    lwz r6, 0x54(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F620:
    lwz r6, 0x58(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F634:
    lwz r6, 0x5c(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F648:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F65C:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F670:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F684:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F698:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F6AC:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F6C0:
    lwz r6, 0x48(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F6D4:
    lwz r6, 0x4c(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F6E8:
    lwz r6, 0x50(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F6FC:
    lwz r6, 0x54(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F710:
    lwz r6, 0x58(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F724:
    lwz r6, 0x5c(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F738:
    lwz r6, 0x48(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F74C:
    lwz r6, 0x4c(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F760:
    lwz r6, 0x50(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F774:
    lwz r6, 0x54(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F788:
    lwz r6, 0x58(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F79C:
    lwz r6, 0x5c(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F7B0:
    lwz r6, 0x48(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F7C4:
    lwz r6, 0x4c(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F7D8:
    lwz r6, 0x50(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F7EC:
    lwz r6, 0x54(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F800:
    lwz r6, 0x58(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F814:
    lwz r6, 0x5c(r5)
    li r5, 0x3
    li r7, 0x2
    bl fn_80061B74
    b @L_8005FFD4
@L_8005F828:
    lwz r6, 0x0(r5)
    li r5, 0x0
    li r7, 0x0
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F83C:
    lwz r6, 0x4(r5)
    li r5, 0x0
    li r7, 0x0
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F850:
    lwz r6, 0x8(r5)
    li r5, 0x0
    li r7, 0x0
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F864:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F878:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F88C:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F8A0:
    lwz r6, 0x0(r5)
    li r5, 0x0
    li r7, 0x0
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F8B4:
    lwz r6, 0x4(r5)
    li r5, 0x0
    li r7, 0x0
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F8C8:
    lwz r6, 0x8(r5)
    li r5, 0x0
    li r7, 0x0
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F8DC:
    lwz r6, 0x0(r5)
    li r5, 0x0
    li r7, 0x0
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F8F0:
    lwz r6, 0x4(r5)
    li r5, 0x0
    li r7, 0x0
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F904:
    lwz r6, 0x8(r5)
    li r5, 0x0
    li r7, 0x0
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F918:
    lwz r6, 0x18(r5)
    li r5, 0x1
    li r7, 0x0
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F92C:
    lwz r6, 0x1c(r5)
    li r5, 0x1
    li r7, 0x0
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F940:
    lwz r6, 0x20(r5)
    li r5, 0x1
    li r7, 0x0
    bl fn_8006106C
    b @L_8005FFD4
@L_8005F954:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F968:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F97C:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005F990:
    lwz r6, 0x18(r5)
    li r5, 0x1
    li r7, 0x0
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F9A4:
    lwz r6, 0x1c(r5)
    li r5, 0x1
    li r7, 0x0
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F9B8:
    lwz r6, 0x20(r5)
    li r5, 0x1
    li r7, 0x0
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005F9CC:
    lwz r6, 0x18(r5)
    li r5, 0x1
    li r7, 0x0
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F9E0:
    lwz r6, 0x1c(r5)
    li r5, 0x1
    li r7, 0x0
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005F9F4:
    lwz r6, 0x20(r5)
    li r5, 0x1
    li r7, 0x0
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FA08:
    lwz r6, 0x0(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FA1C:
    lwz r6, 0x4(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FA30:
    lwz r6, 0x8(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FA44:
    lwz r6, 0xc(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FA58:
    lwz r6, 0x10(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FA6C:
    lwz r6, 0x14(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FA80:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FA94:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FAA8:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FABC:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FAD0:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FAE4:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FAF8:
    lwz r6, 0x0(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FB0C:
    lwz r6, 0x4(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FB20:
    lwz r6, 0x8(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FB34:
    lwz r6, 0xc(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FB48:
    lwz r6, 0x10(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FB5C:
    lwz r6, 0x14(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FB70:
    lwz r6, 0x0(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FB84:
    lwz r6, 0x4(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FB98:
    lwz r6, 0x8(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FBAC:
    lwz r6, 0xc(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FBC0:
    lwz r6, 0x10(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FBD4:
    lwz r6, 0x14(r5)
    li r5, 0x0
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FBE8:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FBFC:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FC10:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FC24:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FC38:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FC4C:
    lbz r0, 0x4(r4)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r4)
    b @L_8005FFD4
@L_8005FC60:
    lwz r6, 0x18(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FC74:
    lwz r6, 0x1c(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FC88:
    lwz r6, 0x20(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FC9C:
    lwz r6, 0x24(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FCB0:
    lwz r6, 0x28(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FCC4:
    lwz r6, 0x2c(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061A2C
    b @L_8005FFD4
@L_8005FCD8:
    lwz r6, 0x18(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FCEC:
    lwz r6, 0x1c(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FD00:
    lwz r6, 0x20(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FD14:
    lwz r6, 0x24(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FD28:
    lwz r6, 0x28(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FD3C:
    lwz r6, 0x2c(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_8006106C
    b @L_8005FFD4
@L_8005FD50:
    lwz r6, 0x18(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FD64:
    lwz r6, 0x1c(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FD78:
    lwz r6, 0x20(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FD8C:
    lwz r6, 0x24(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FDA0:
    lwz r6, 0x28(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FDB4:
    lwz r6, 0x2c(r5)
    li r5, 0x1
    li r7, 0x1
    bl fn_80061BBC
    b @L_8005FFD4
@L_8005FDC8:
    li r5, 0x0
    li r6, 0x0
    bl fn_80060D70
    b @L_8005FFD4
@L_8005FDD8:
    li r5, 0x0
    li r6, 0x1
    bl fn_80060D70
    b @L_8005FFD4
@L_8005FDE8:
    li r5, 0x1
    li r6, 0x0
    bl fn_80060D70
    b @L_8005FFD4
@L_8005FDF8:
    li r5, 0x1
    li r6, 0x1
    bl fn_80060D70
    b @L_8005FFD4
@L_8005FE08:
    li r5, 0x1
    li r6, 0x2
    bl fn_80060D70
    b @L_8005FFD4
@L_8005FE18:
    li r5, 0x1
    li r6, 0x2
    bl fn_80060D70
    b @L_8005FFD4
@L_8005FE28:
    li r5, 0x6
    bl fn_80060EF4
    b @L_8005FFD4
@L_8005FE34:
    li r5, 0x6
    bl fn_80060EF4
    b @L_8005FFD4
@L_8005FE40:
    li r5, -0x1
    bl fn_80060EF4
    b @L_8005FFD4
@L_8005FE4C:
    li r5, 0x3
    bl fn_80060EF4
    b @L_8005FFD4
@L_8005FE58:
    li r5, 0x4
    bl fn_80060EF4
    b @L_8005FFD4
@L_8005FE64:
    li r5, 0x2
    bl fn_80060EF4
    b @L_8005FFD4
@L_8005FE70:
    li r5, 0x1
    bl fn_80060EF4
    b @L_8005FFD4
@L_8005FE7C:
    li r5, 0x0
    bl fn_80060EF4
    b @L_8005FFD4
@L_8005FE88:
    li r5, 0x0
    li r6, 0x0
    bl fn_800617E0
    b @L_8005FFD4
@L_8005FE98:
    li r5, 0x1
    li r6, 0x0
    bl fn_800617E0
    b @L_8005FFD4
@L_8005FEA8:
    li r5, 0x0
    li r6, 0x0
    bl fn_800615F4
    b @L_8005FFD4
@L_8005FEB8:
    li r5, 0x1
    li r6, 0x0
    bl fn_800615F4
    b @L_8005FFD4
@L_8005FEC8:
    li r5, 0x0
    li r6, 0x0
    bl fn_80061454
    b @L_8005FFD4
@L_8005FED8:
    li r5, 0x1
    li r6, 0x0
    bl fn_80061454
    b @L_8005FFD4
@L_8005FEE8:
    li r5, 0x0
    li r6, 0x2
    bl fn_800617E0
    b @L_8005FFD4
@L_8005FEF8:
    li r5, 0x1
    li r6, 0x2
    bl fn_800617E0
    b @L_8005FFD4
@L_8005FF08:
    li r5, 0x2
    li r6, 0x2
    bl fn_800617E0
    b @L_8005FFD4
@L_8005FF18:
    li r5, 0x3
    li r6, 0x2
    bl fn_800617E0
    b @L_8005FFD4
@L_8005FF28:
    li r5, 0x0
    li r6, 0x2
    bl fn_80061454
    b @L_8005FFD4
@L_8005FF38:
    li r5, 0x1
    li r6, 0x2
    bl fn_80061454
    b @L_8005FFD4
@L_8005FF48:
    li r5, 0x2
    li r6, 0x2
    bl fn_80061454
    b @L_8005FFD4
@L_8005FF58:
    li r5, 0x3
    li r6, 0x2
    bl fn_80061454
    b @L_8005FFD4
@L_8005FF68:
    li r5, 0x0
    li r6, 0x2
    bl fn_800615F4
    b @L_8005FFD4
@L_8005FF78:
    li r5, 0x1
    li r6, 0x2
    bl fn_800615F4
    b @L_8005FFD4
@L_8005FF88:
    li r5, 0x2
    li r6, 0x2
    bl fn_800615F4
    b @L_8005FFD4
@L_8005FF98:
    li r5, 0x3
    li r6, 0x2
    bl fn_800615F4
    b @L_8005FFD4
@L_8005FFA8:
    lis r5, lbl_803A9A60@ha
    addi r5, r5, lbl_803A9A60@l
    lfs f1, 0x48(r5)
    bl fn_800609B4
    b @L_8005FFD4
@L_8005FFBC:
    lis r5, lbl_803A9A60@ha
    addi r5, r5, lbl_803A9A60@l
    lfs f1, 0x4c(r5)
    bl fn_800609B4
    b @L_8005FFD4
@L_8005FFD0:
    bl fn_80060434
@L_8005FFD4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
