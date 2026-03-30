    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr. r29, r3
    bne @L_801B5E78
    addi r3, r2, -22576
    li r4, 0x366
    addi r5, r2, -22544
    bl fn_80196E10
@L_801B5E78:
    cmplwi r29, 0x0
    bne @L_801B5E88
    li r0, 0x0
    b @L_801B5EB4
@L_801B5E88:
    addis r0, r29, 0x1
    cmplwi r0, 0xffff
    bne @L_801B5E9C
    li r0, 0x2
    b @L_801B5EB4
@L_801B5E9C:
    addis r0, r29, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B5EB0
    li r0, 0x3
    b @L_801B5EB4
@L_801B5EB0:
    lwz r0, 0x0(r29)
@L_801B5EB4:
    cmpwi r0, 0x1
    beq @L_801B5ED0
    lis r4, -32729
    addi r3, r2, -22576
    addi r5, r4, 21468
    li r4, 0x367
    bl fn_80196E10
@L_801B5ED0:
    cmpwi r31, 0xff
    stw r30, 0x74(r29)
    bne @L_801B5EE8
    li r0, 0xff
    stb r0, 0x78(r29)
    b @L_801B5EEC
@L_801B5EE8:
    stb r31, 0x78(r29)
@L_801B5EEC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
