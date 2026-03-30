    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r4, lbl_8047AA80(r13)
    lbz r0, 0x47e(r4)
    cmplwi r0, 0x1
    bne @L_800D67E8
    bl fn_800DB758
    b @L_800D69EC
    @L_800D67E8:
    lwz r0, 0x0(r4)
    cmpwi r0, 0x1
    bne @L_800D680C
    clrlwi r5, r31, 16
    li r3, 0x2
    li r4, 0x1
    crxor 6, 6, 6
    bl fn_800D4F98
    b @L_800D69EC
    @L_800D680C:
    lbz r3, 0x1b(r4)
    lbz r0, 0x1a(r4)
    cmplw r3, r0
    bne @L_800D69EC
    lwz r3, 0x4(r4)
    lwz r0, 0x8(r4)
    and. r0, r3, r0
    beq @L_800D69EC
    lwz r0, 0x24(r4)
    cmplwi r0, 0x0
    bne @L_800D6910
    lwz r3, 0x20(r4)
    bl fn_800D7650
    lwz r3, lbl_8047AA80(r13)
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    lwz r3, 0x20(r3)
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x10(r3)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_800D689C
    lwz r3, 0x20(r3)
    li r4, 0x2
    li r5, 0x0
    li r6, 0x2
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    @L_800D689C:
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x10(r3)
    clrlwi. r0, r0, 31
    beq @L_800D68D0
    lwz r3, 0x20(r3)
    li r4, 0x4
    li r5, 0x0
    li r6, 0x6
    li r7, 0xa
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    @L_800D68D0:
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x10(r3)
    rlwinm. r0, r0, 0, 30, 30
    beq @L_800D6904
    lwz r3, 0x20(r3)
    li r4, 0x6
    li r5, 0x0
    li r6, 0x8
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    @L_800D6904:
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x20(r3)
    stw r0, 0x24(r3)
    @L_800D6910:
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x14(r3)
    cmpwi r0, 0x7
    bne @L_800D6924
    clrlslwi r31, r31, 17, 1
    @L_800D6924:
    lwz r3, 0x24(r3)
    bl fn_800D7A70
    lwz r3, lbl_8047AA80(r13)
    lwz r3, 0x24(r3)
    bl fn_800D892C
    lwz r4, lbl_8047AA80(r13)
    lis r3, lbl_80314350@ha
    addi r3, r3, lbl_80314350@l
    mr r5, r31
    lwz r0, 0x14(r4)
    lwz r4, 0x24(r4)
    slwi r0, r0, 2
    lwzx r3, r3, r0
    lwz r4, 0x4(r4)
    bl fn_800B928C
    lwz r4, lbl_8047AA80(r13)
    lis r3, lbl_804001F0@ha
    addi r6, r3, lbl_804001F0@l
    clrlwi r5, r31, 16
    lwz r3, 0x14(r4)
    lwz r0, 0xc(r6)
    cmpwi r3, 0x6
    add r0, r0, r5
    stw r0, 0xc(r6)
    bge @L_800D6998
    cmpwi r3, 0x3
    beq @L_800D69A4
    bge @L_800D69C8
    b @L_800D69EC
    @L_800D6998:
    cmpwi r3, 0x8
    bge @L_800D69EC
    b @L_800D69DC
    @L_800D69A4:
    lis r3, 0x5555
    lwz r4, 0x4(r6)
    addi r0, r3, 0x5556
    mulhw r3, r0, r5
    srwi r0, r3, 31
    add r0, r3, r0
    add r0, r4, r0
    stw r0, 0x4(r6)
    b @L_800D69EC
    @L_800D69C8:
    lwz r0, 0x4(r6)
    add r3, r5, r0
    subi r0, r3, 0x2
    stw r0, 0x4(r6)
    b @L_800D69EC
    @L_800D69DC:
    lwz r3, 0x4(r6)
    extrwi r0, r31, 15, 16
    add r0, r3, r0
    stw r0, 0x4(r6)
    @L_800D69EC:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
