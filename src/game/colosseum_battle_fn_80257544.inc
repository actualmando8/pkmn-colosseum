    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x3
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    li r30, 0x0
    mr r31, r3
    li r4, 0x4a
    bl fn_801F6E98
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80257588
    mr r3, r31
    li r4, 0x4a
    bl fn_801F6D9C
    mr r30, r3
@L_80257588:
    li r3, 0x4a
    bl fn_80119DD0
    clrlwi r3, r3, 24
    extsh r0, r30
    cmpw r0, r3
    blt @L_802575A8
    li r3, 0x0
    b @L_802575AC
@L_802575A8:
    li r3, 0x1
@L_802575AC:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
