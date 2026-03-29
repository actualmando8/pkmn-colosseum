    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x10e
    li r4, 0x1f
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    mr r3, r31
    li r4, 0x1
    bl fn_80056B74
    b @L_80055BE0
    @L_80055BDC:
    bl fn_800F0308
    @L_80055BE0:
    mr r3, r31
    bl fn_80055E38
    cmpwi r3, 0x0
    beq @L_80055BDC
    li r3, 0x0
    li r4, 0x1
    bl fn_80054760
    li r3, 0x0
    li r4, 0x0
    bl fn_80057A64
    bl fn_800558B8
    bl fn_80057A38
    bl fn_8005471C
    bl fn_80056A80
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
