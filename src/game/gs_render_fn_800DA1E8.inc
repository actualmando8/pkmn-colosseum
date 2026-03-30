    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r7, r5
    stw r0, 0x14(r1)
    lwz r6, lbl_8047AA80(r13)
    lwz r0, 0x0(r6)
    cmpwi r0, 0x1
    bne @L_800DA224
    mr r5, r3
    mr r6, r4
    li r3, 0x2f
    li r4, 0x3
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800DA2AC
    @L_800DA224:
    cmpwi r3, 0x1
    bne @L_800DA238
    li r0, 0x1
    stb r0, 0x41c(r6)
    b @L_800DA248
    @L_800DA238:
    cmpwi r3, 0x0
    bne @L_800DA248
    li r0, 0x0
    stb r0, 0x41c(r6)
    @L_800DA248:
    cmpwi r7, 0x1
    bne @L_800DA260
    lwz r3, lbl_8047AA80(r13)
    li r0, 0x1
    stb r0, 0x424(r3)
    b @L_800DA274
    @L_800DA260:
    cmpwi r7, 0x0
    bne @L_800DA274
    lwz r3, lbl_8047AA80(r13)
    li r0, 0x0
    stb r0, 0x424(r3)
    @L_800DA274:
    lis r3, lbl_8031454C@ha
    slwi r0, r4, 2
    addi r4, r3, lbl_8031454C@l
    lwz r3, lbl_8047AA80(r13)
    lwzx r0, r4, r0
    stw r0, 0x420(r3)
    lwz r5, lbl_8047AA80(r13)
    lbz r3, 0x41c(r5)
    lwz r4, 0x420(r5)
    lbz r5, 0x41b(r5)
    bl fn_800BCE88
    lwz r3, lbl_8047AA80(r13)
    lbz r3, 0x424(r3)
    bl fn_800BCEBC
    @L_800DA2AC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
