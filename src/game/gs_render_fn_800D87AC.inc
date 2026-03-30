    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    rlwinm. r0, r3, 0, 30, 30
    stmw r27, 0xc(r1)
    mr r27, r3
    lwz r4, lbl_8047AA80(r13)
    lwz r3, 0x414(r4)
    or r0, r3, r27
    stw r0, 0x414(r4)
    beq @L_800D8828
    lis r3, lbl_80314404@ha
    li r30, 0x0
    addi r29, r3, lbl_80314404@l
    li r28, 0x0
    mr r31, r30
    @L_800D87EC:
    lwz r3, lbl_8047AA80(r13)
    addi r0, r30, 0x28
    addi r5, r28, 0x4
    li r4, 0x1
    stwx r31, r3, r0
    li r6, 0x3c
    li r7, 0x0
    li r8, 0x7d
    lwz r3, 0x0(r29)
    bl fn_800B857C
    addi r28, r28, 0x1
    addi r29, r29, 0x4
    cmpwi r28, 0x8
    addi r30, r30, 0x4
    blt @L_800D87EC
    @L_800D8828:
    rlwinm. r0, r27, 0, 29, 29
    beq @L_800D8878
    li r29, 0x0
    mr r31, r29
    @L_800D8838:
    lwz r3, lbl_8047AA80(r13)
    addi r4, r29, 0x25c
    lbzx r0, r3, r4
    cmplwi r0, 0x0
    beq @L_800D8858
    stbx r31, r3, r4
    mr r3, r29
    bl fn_800BBC34
    @L_800D8858:
    addi r29, r29, 0x1
    cmpwi r29, 0x10
    blt @L_800D8838
    lwz r4, lbl_8047AA80(r13)
    li r0, 0x0
    li r3, 0x0
    stb r0, 0x3ac(r4)
    bl fn_800BBC0C
    @L_800D8878:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
