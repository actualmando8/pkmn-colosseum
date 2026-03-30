    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r31, r5
    mr r30, r3
    li r5, 0xee
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_8023200C
    cmplwi r31, 0x0
    beq @L_80232004
    stw r30, 0x0(r31)
@L_80232004:
    li r3, 0x0
    b @L_80232010
@L_8023200C:
    li r3, 0x1
@L_80232010:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
