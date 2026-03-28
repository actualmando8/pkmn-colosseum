    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lha r0, 0x6(r4)
    cmpwi r0, 0xe26
    beq @L_80063A40
    bge @L_8006394C
    cmpwi r0, 0xe16
    beq @L_800639E0
    bge @L_80063934
    cmpwi r0, 0xe14
    beq @L_800639A0
    bge @L_800639C0
    cmpwi r0, 0xe08
    beq @L_80063988
    b @L_80063AC4
@L_80063934:
    cmpwi r0, 0xe24
    beq @L_80063A00
    bge @L_80063A20
    cmpwi r0, 0xe18
    bge @L_80063AC4
    b @L_80063990
@L_8006394C:
    cmpwi r0, 0x1264
    beq @L_80063998
    bge @L_80063970
    cmpwi r0, 0x1123
    beq @L_80063AC0
    bge @L_80063AC4
    cmpwi r0, 0xe28
    bge @L_80063AC4
    b @L_80063A60
@L_80063970:
    cmpwi r0, 0x1270
    beq @L_80063AA0
    bge @L_80063AC4
    cmpwi r0, 0x126f
    bge @L_80063A80
    b @L_80063AC4
@L_80063988:
    bl fn_80063AD4
    b @L_80063AC4
@L_80063990:
    bl fn_80063AD4
    b @L_80063AC4
@L_80063998:
    bl fn_80063AD4
    b @L_80063AC4
@L_800639A0:
    lbz r5, 0x8b(r3)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3c21
    bl fn_800FB680
    b @L_80063AC4
@L_800639C0:
    lbz r5, 0x8b(r3)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3db2
    bl fn_800FB680
    b @L_80063AC4
@L_800639E0:
    lbz r5, 0x8b(r3)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3db3
    bl fn_800FB680
    b @L_80063AC4
@L_80063A00:
    lbz r5, 0x8b(r3)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3c21
    bl fn_800FB680
    b @L_80063AC4
@L_80063A20:
    lbz r5, 0x8b(r3)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3dae
    bl fn_800FB680
    b @L_80063AC4
@L_80063A40:
    lbz r5, 0x8b(r3)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3db2
    bl fn_800FB680
    b @L_80063AC4
@L_80063A60:
    lbz r5, 0x8b(r3)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3db3
    bl fn_800FB680
    b @L_80063AC4
@L_80063A80:
    lbz r5, 0x8b(r3)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3c21
    bl fn_800FB680
    b @L_80063AC4
@L_80063AA0:
    lbz r5, 0x8b(r3)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0x3db3
    bl fn_800FB680
    b @L_80063AC4
@L_80063AC0:
    bl fn_80063AD4
@L_80063AC4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
