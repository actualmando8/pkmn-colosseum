    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stw r31, 0x8c(r1)
    stw r30, 0x88(r1)
    stw r29, 0x84(r1)
    stw r28, 0x80(r1)
    bne cr1, @L_8002A0F8
    stfd f1, 0x28(r1)
    stfd f2, 0x30(r1)
    stfd f3, 0x38(r1)
    stfd f4, 0x40(r1)
    stfd f5, 0x48(r1)
    stfd f6, 0x50(r1)
    stfd f7, 0x58(r1)
    stfd f8, 0x60(r1)
    @L_8002A0F8:
    stw r3, 0x8(r1)
    stw r4, 0xc(r1)
    stw r5, 0x10(r1)
    stw r6, 0x14(r1)
    stw r7, 0x18(r1)
    stw r8, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r10, 0x24(r1)
    addi r0, r1, 0x98
    addi r8, r1, 0x8
    lis r9, 0x400
    stw r0, 0x6c(r1)
    slwi r0, r4, 2
    lwz r7, lbl_80478E54(r13)
    stw r9, 0x68(r1)
    slwi r31, r5, 2
    lwz r4, lbl_80478E3C(r13)
    li r30, 0x1
    stw r8, 0x70(r1)
    lbzx r0, r7, r0
    mulli r5, r0, 0x3c
    lbzx r0, r4, r5
    addi r28, r5, 0x4
    stb r0, 0x0(r3)
    lwz r0, lbl_80478E3C(r13)
    add r28, r0, r28
    b @L_8002A198
    @L_8002A164:
    cmpwi r30, 0x0
    beq @L_8002A178
    mr r29, r6
    li r30, 0x0
    b @L_8002A188
    @L_8002A178:
    mr r3, r29
    mr r4, r6
    li r30, 0x1
    bl fn_80132A38
    @L_8002A188:
    addi r3, r1, 0x68
    li r4, 0x1
    bl __va_arg
    lwz r6, 0x0(r3)
    @L_8002A198:
    cmpwi r6, 0x0
    bge @L_8002A164
    lwzx r3, r28, r31
    lwz r0, 0x94(r1)
    lwz r31, 0x8c(r1)
    lwz r30, 0x88(r1)
    lwz r29, 0x84(r1)
    lwz r28, 0x80(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
