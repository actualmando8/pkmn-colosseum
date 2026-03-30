    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0x102
    li r6, 0x0
    bl fn_8012640C
    extsh r30, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0x104
    li r6, 0x0
    bl fn_8012640C
    extsh. r0, r30
    extsh r0, r3
    bne @L_802162C0
    extsh. r0, r0
    beq @L_802162D0
@L_802162C0:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_802162DC
@L_802162D0:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_802162DC:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
