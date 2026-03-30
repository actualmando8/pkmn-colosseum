    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    bl fn_80235AA0
    clrlwi r0, r3, 24
    cmplwi r0, 0xc
    bge @L_8025650C
    lfs f1, -20564(r2)
    mr r3, r30
    mr r4, r31
    li r5, -0x1
    bl fn_802373B0
    clrlwi. r0, r3, 24
    beq @L_80256514
@L_8025650C:
    li r3, 0x0
    b @L_80256518
@L_80256514:
    li r3, 0x1
@L_80256518:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
