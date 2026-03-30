    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lha r0, 0x1a(r3)
    lwz r31, 0x24(r3)
    cmpwi r0, 0x0
    blt @L_801DF45C
    lbz r0, 0x19(r30)
    cmplwi r0, 0x6
    beq @L_801DF45C
    cmplwi r0, 0x1
    beq @L_801DF45C
    li r0, 0x1
    mr r3, r31
    stb r0, 0x19(r30)
    li r4, 0x0
    bl fn_800EC188
    lha r4, 0x1a(r30)
    mr r3, r31
    bl fn_800EC35C
    lfs f1, -21208(r2)
    mr r3, r31
    bl fn_800EC308
    mr r3, r31
    li r4, 0x0
    bl fn_800EC208
    lfs f1, -21204(r2)
    mr r3, r31
    bl fn_800EC2A4
    mr r3, r31
    bl fn_800EC1E4
@L_801DF45C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
