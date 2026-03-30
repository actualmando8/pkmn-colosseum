    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_801DA340
    bl fn_801DD028
    mr r3, r31
    bl fn_801DCF84
    mr r3, r31
    bl fn_801DCEA8
    cmplwi r31, 0x0
    beq @L_801DA340
    lwz r3, 0x24(r31)
    cmplwi r3, 0x0
    beq @L_801DA340
    lbz r0, 0x18(r31)
    li r4, 0x1
    ori r0, r0, 0x1
    stb r0, 0x18(r31)
    bl fn_800E4014
    lwz r3, 0x80(r31)
    cmplwi r3, 0x0
    beq @L_801DA32C
    li r4, 0x1
    bl fn_80118C88
@L_801DA32C:
    lwz r3, 0x84(r31)
    cmplwi r3, 0x0
    beq @L_801DA340
    li r4, 0x1
    bl fn_80118C88
@L_801DA340:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
