    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x2d
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_80236BFC
    clrlwi. r0, r3, 24
    bne @L_80255C44
    li r31, 0x0
    b @L_80255C54
@L_80255C44:
    mr r3, r31
    li r4, 0x2d
    bl fn_80202360
    mr r31, r3
@L_80255C54:
    li r3, 0x2d
    bl fn_80119DD0
    clrlwi r3, r3, 24
    extsh r0, r31
    cmpw r0, r3
    blt @L_80255C74
    li r3, 0x0
    b @L_80255C78
@L_80255C74:
    li r3, 0x1
@L_80255C78:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
