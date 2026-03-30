    nofralloc
    stwu r1, -0x20(r1)
    cmpwi r4, 0x2
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    beq @L_800D9928
    bge @L_800D9668
    cmpwi r4, 0x0
    beq @L_800D9678
    bge @L_800D975C
    b @L_800D9ADC
    @L_800D9668:
    cmpwi r4, 0x4
    beq @L_800D9A04
    bge @L_800D9ADC
    b @L_800D9840
    @L_800D9678:
    mulli r4, r3, 0x5
    slwi r3, r3, 2
    lwz r0, lbl_8047AA80(r13)
    li r11, 0x0
    li r10, 0x1
    addi r8, r4, 0xab
    add r8, r0, r8
    addi r12, r3, 0x14b
    stb r11, 0x0(r8)
    li r7, 0xf
    li r6, 0xa
    li r5, 0x8
    stb r11, 0x1(r8)
    addi r31, r4, 0xfb
    addi r30, r3, 0x18b
    li r4, 0x7
    stb r11, 0x2(r8)
    li r3, 0x4
    li r0, 0x5
    stb r10, 0x3(r8)
    stb r11, 0x4(r8)
    lwz r9, lbl_8047AA80(r13)
    lwz r8, 0x414(r9)
    ori r8, r8, 0x4
    stw r8, 0x414(r9)
    lwz r8, lbl_8047AA80(r13)
    add r12, r8, r12
    stb r7, 0x0(r12)
    stb r6, 0x1(r12)
    stb r5, 0x2(r12)
    stb r7, 0x3(r12)
    lwz r6, lbl_8047AA80(r13)
    lwz r5, 0x414(r6)
    ori r5, r5, 0x4
    stw r5, 0x414(r6)
    lwz r5, lbl_8047AA80(r13)
    add r31, r5, r31
    stb r11, 0x0(r31)
    stb r11, 0x1(r31)
    stb r11, 0x2(r31)
    stb r10, 0x3(r31)
    stb r11, 0x4(r31)
    lwz r6, lbl_8047AA80(r13)
    lwz r5, 0x414(r6)
    ori r5, r5, 0x4
    stw r5, 0x414(r6)
    lwz r5, lbl_8047AA80(r13)
    add r30, r5, r30
    stb r4, 0x0(r30)
    stb r3, 0x1(r30)
    stb r0, 0x2(r30)
    stb r4, 0x3(r30)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x414(r3)
    ori r0, r0, 0x4
    stw r0, 0x414(r3)
    b @L_800D9ADC
    @L_800D975C:
    mulli r9, r3, 0x5
    slwi r3, r3, 2
    lwz r0, lbl_8047AA80(r13)
    li r11, 0x0
    li r10, 0x1
    addi r8, r9, 0xab
    add r8, r0, r8
    addi r12, r3, 0x14b
    stb r11, 0x0(r8)
    li r7, 0xa
    li r6, 0x8
    li r5, 0x9
    stb r11, 0x1(r8)
    li r4, 0xf
    addi r30, r9, 0xfb
    addi r31, r3, 0x18b
    stb r11, 0x2(r8)
    li r3, 0x7
    li r0, 0x5
    stb r10, 0x3(r8)
    stb r11, 0x4(r8)
    lwz r9, lbl_8047AA80(r13)
    lwz r8, 0x414(r9)
    ori r8, r8, 0x4
    stw r8, 0x414(r9)
    lwz r8, lbl_8047AA80(r13)
    add r12, r8, r12
    stb r7, 0x0(r12)
    stb r6, 0x1(r12)
    stb r5, 0x2(r12)
    stb r4, 0x3(r12)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x414(r5)
    ori r4, r4, 0x4
    stw r4, 0x414(r5)
    lwz r4, lbl_8047AA80(r13)
    add r30, r4, r30
    stb r11, 0x0(r30)
    stb r11, 0x1(r30)
    stb r11, 0x2(r30)
    stb r10, 0x3(r30)
    stb r11, 0x4(r30)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x414(r5)
    ori r4, r4, 0x4
    stw r4, 0x414(r5)
    lwz r4, lbl_8047AA80(r13)
    add r31, r4, r31
    stb r3, 0x0(r31)
    stb r3, 0x1(r31)
    stb r3, 0x2(r31)
    stb r0, 0x3(r31)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x414(r3)
    ori r0, r0, 0x4
    stw r0, 0x414(r3)
    b @L_800D9ADC
    @L_800D9840:
    mulli r4, r3, 0x5
    slwi r3, r3, 2
    lwz r0, lbl_8047AA80(r13)
    li r12, 0x0
    li r11, 0x1
    addi r9, r4, 0xab
    add r9, r0, r9
    addi r30, r3, 0x14b
    stb r12, 0x0(r9)
    li r8, 0xa
    li r7, 0xc
    li r6, 0x8
    stb r12, 0x1(r9)
    li r5, 0xf
    addi r31, r4, 0xfb
    addi r29, r3, 0x18b
    stb r12, 0x2(r9)
    li r4, 0x7
    li r3, 0x5
    li r0, 0x4
    stb r11, 0x3(r9)
    stb r12, 0x4(r9)
    lwz r10, lbl_8047AA80(r13)
    lwz r9, 0x414(r10)
    ori r9, r9, 0x4
    stw r9, 0x414(r10)
    lwz r9, lbl_8047AA80(r13)
    add r30, r9, r30
    stb r8, 0x0(r30)
    stb r7, 0x1(r30)
    stb r6, 0x2(r30)
    stb r5, 0x3(r30)
    lwz r6, lbl_8047AA80(r13)
    lwz r5, 0x414(r6)
    ori r5, r5, 0x4
    stw r5, 0x414(r6)
    lwz r5, lbl_8047AA80(r13)
    add r31, r5, r31
    stb r12, 0x0(r31)
    stb r12, 0x1(r31)
    stb r12, 0x2(r31)
    stb r11, 0x3(r31)
    stb r12, 0x4(r31)
    lwz r6, lbl_8047AA80(r13)
    lwz r5, 0x414(r6)
    ori r5, r5, 0x4
    stw r5, 0x414(r6)
    lwz r5, lbl_8047AA80(r13)
    add r29, r5, r29
    stb r4, 0x0(r29)
    stb r3, 0x1(r29)
    stb r0, 0x2(r29)
    stb r4, 0x3(r29)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x414(r3)
    ori r0, r0, 0x4
    stw r0, 0x414(r3)
    b @L_800D9ADC
    @L_800D9928:
    mulli r7, r3, 0x5
    slwi r3, r3, 2
    lwz r0, lbl_8047AA80(r13)
    li r9, 0x0
    li r8, 0x1
    addi r6, r7, 0xab
    add r6, r0, r6
    addi r10, r3, 0x14b
    stb r9, 0x0(r6)
    li r5, 0xf
    li r4, 0x8
    addi r11, r7, 0xfb
    stb r9, 0x1(r6)
    addi r12, r3, 0x18b
    li r3, 0x7
    li r0, 0x4
    stb r9, 0x2(r6)
    stb r8, 0x3(r6)
    stb r9, 0x4(r6)
    lwz r7, lbl_8047AA80(r13)
    lwz r6, 0x414(r7)
    ori r6, r6, 0x4
    stw r6, 0x414(r7)
    lwz r6, lbl_8047AA80(r13)
    add r10, r6, r10
    stb r5, 0x0(r10)
    stb r5, 0x1(r10)
    stb r5, 0x2(r10)
    stb r4, 0x3(r10)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x414(r5)
    ori r4, r4, 0x4
    stw r4, 0x414(r5)
    lwz r4, lbl_8047AA80(r13)
    add r11, r4, r11
    stb r9, 0x0(r11)
    stb r9, 0x1(r11)
    stb r9, 0x2(r11)
    stb r8, 0x3(r11)
    stb r9, 0x4(r11)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x414(r5)
    ori r4, r4, 0x4
    stw r4, 0x414(r5)
    lwz r4, lbl_8047AA80(r13)
    add r12, r4, r12
    stb r3, 0x0(r12)
    stb r3, 0x1(r12)
    stb r3, 0x2(r12)
    stb r0, 0x3(r12)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x414(r3)
    ori r0, r0, 0x4
    stw r0, 0x414(r3)
    b @L_800D9ADC
    @L_800D9A04:
    mulli r7, r3, 0x5
    slwi r3, r3, 2
    lwz r0, lbl_8047AA80(r13)
    li r9, 0x0
    li r8, 0x1
    addi r6, r7, 0xab
    add r6, r0, r6
    addi r10, r3, 0x14b
    stb r9, 0x0(r6)
    li r5, 0xf
    li r4, 0xa
    addi r11, r7, 0xfb
    stb r9, 0x1(r6)
    addi r12, r3, 0x18b
    li r3, 0x7
    li r0, 0x5
    stb r9, 0x2(r6)
    stb r8, 0x3(r6)
    stb r9, 0x4(r6)
    lwz r7, lbl_8047AA80(r13)
    lwz r6, 0x414(r7)
    ori r6, r6, 0x4
    stw r6, 0x414(r7)
    lwz r6, lbl_8047AA80(r13)
    add r10, r6, r10
    stb r5, 0x0(r10)
    stb r5, 0x1(r10)
    stb r5, 0x2(r10)
    stb r4, 0x3(r10)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x414(r5)
    ori r4, r4, 0x4
    stw r4, 0x414(r5)
    lwz r4, lbl_8047AA80(r13)
    add r11, r4, r11
    stb r9, 0x0(r11)
    stb r9, 0x1(r11)
    stb r9, 0x2(r11)
    stb r8, 0x3(r11)
    stb r9, 0x4(r11)
    lwz r5, lbl_8047AA80(r13)
    lwz r4, 0x414(r5)
    ori r4, r4, 0x4
    stw r4, 0x414(r5)
    lwz r4, lbl_8047AA80(r13)
    add r12, r4, r12
    stb r3, 0x0(r12)
    stb r3, 0x1(r12)
    stb r3, 0x2(r12)
    stb r0, 0x3(r12)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0x414(r3)
    ori r0, r0, 0x4
    stw r0, 0x414(r3)
    @L_800D9ADC:
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    blr
