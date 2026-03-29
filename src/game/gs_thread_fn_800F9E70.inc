    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    cmplwi r30, 0x0
    bne @L_800F9E9C
    li r3, 0x0
    b @L_800F9ECC
@L_800F9E9C:
    cmplwi r31, 0x0
    bne @L_800F9EB0
    li r0, 0x0
    sth r0, 0x0(r30)
    b @L_800F9EC8
@L_800F9EB0:
    mr r3, r31
    bl fn_800FE010
    mr r5, r3
    mr r3, r30
    mr r4, r31
    bl memcpy
@L_800F9EC8:
    mr r3, r30
@L_800F9ECC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
