    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x10
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r31, r6
    mr r30, r3
    mr r4, r31
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80259ACC
    mr r3, r30
    mr r4, r31
    bl fn_80237288
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80259AD4
@L_80259ACC:
    li r3, 0x0
    b @L_80259AD8
@L_80259AD4:
    li r3, 0x1
@L_80259AD8:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
