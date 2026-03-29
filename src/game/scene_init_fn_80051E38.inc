    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stw r31, 0x7c(r1)
    stw r30, 0x78(r1)
    stw r29, 0x74(r1)
    mr r31, r3
    clrlwi r0, r31, 16
    cmplwi r0, 0x0
    bne @L_80051E68
    li r3, 0x1
    b @L_80053048
    @L_80051E68:
    bl fn_801FC658
    mr r30, r3
    clrlwi r29, r31, 16
    lwz r11, 0x0(r30)
    lwz r10, 0x4(r30)
    lwz r9, 0x8(r30)
    lwz r8, 0xc(r30)
    lwz r7, 0x10(r30)
    lwz r6, 0x14(r30)
    lwz r5, 0x18(r30)
    lwz r4, 0x1c(r30)
    lwz r3, 0x20(r30)
    lwz r0, 0x24(r30)
    stw r11, 0x3c(r1)
    stw r10, 0x40(r1)
    stw r9, 0x44(r1)
    stw r8, 0x48(r1)
    stw r7, 0x4c(r1)
    stw r6, 0x50(r1)
    stw r5, 0x54(r1)
    stw r4, 0x58(r1)
    stw r3, 0x5c(r1)
    stw r0, 0x60(r1)
    @L_80051EC4:
    mr r9, r29
    li r3, 0x86
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_80051F58
    li r3, 0x86
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    lwz r4, 0x3c(r1)
    li r3, -0x1
    lwz r0, 0x40(r1)
    stw r4, 0x0(r30)
    lwz r4, 0x44(r1)
    stw r0, 0x4(r30)
    lwz r0, 0x48(r1)
    stw r4, 0x8(r30)
    lwz r4, 0x4c(r1)
    stw r0, 0xc(r30)
    lwz r0, 0x50(r1)
    stw r4, 0x10(r30)
    lwz r4, 0x54(r1)
    stw r0, 0x14(r30)
    lwz r0, 0x58(r1)
    stw r4, 0x18(r30)
    lwz r4, 0x5c(r1)
    stw r0, 0x1c(r30)
    lwz r0, 0x60(r1)
    stw r4, 0x20(r30)
    stw r0, 0x24(r30)
    b @L_80053048
    @L_80051F58:
    cmpwi r3, -0x2
    bne @L_80051F9C
    li r3, 0x44
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    beq @L_80051F88
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_80051EC4
    @L_80051F88:
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_80053034
    @L_80051F9C:
    li r3, 0x86
    bl fn_801022B8
    cmpwi r3, 0x5b2
    beq @L_800529D4
    bge @L_80052034
    cmpwi r3, 0x5a8
    beq @L_80052224
    bge @L_80051FF8
    cmpwi r3, 0x5a3
    beq @L_800525FC
    bge @L_80051FE0
    cmpwi r3, 0x5a1
    beq @L_8005231C
    bge @L_8005239C
    cmpwi r3, 0x5a0
    bge @L_8005229C
    b @L_80051EC4
    @L_80051FE0:
    cmpwi r3, 0x5a6
    beq @L_80052134
    bge @L_800521AC
    cmpwi r3, 0x5a5
    bge @L_800520BC
    b @L_80052B44
    @L_80051FF8:
    cmpwi r3, 0x5ad
    beq @L_8005267C
    bge @L_8005201C
    cmpwi r3, 0x5ab
    beq @L_8005250C
    bge @L_80052584
    cmpwi r3, 0x5aa
    bge @L_80052494
    b @L_8005241C
    @L_8005201C:
    cmpwi r3, 0x5b0
    beq @L_800528E4
    bge @L_8005295C
    cmpwi r3, 0x5af
    bge @L_8005286C
    b @L_800527F4
    @L_80052034:
    cmpwi r3, 0x60c
    beq @L_80052D44
    bge @L_80052080
    cmpwi r3, 0x608
    beq @L_80052BC4
    bge @L_80052070
    cmpwi r3, 0x5b5
    beq @L_800526F4
    bge @L_80052064
    cmpwi r3, 0x5b4
    bge @L_80052A4C
    b @L_80052ACC
    @L_80052064:
    cmpwi r3, 0x5b7
    bge @L_80051EC4
    b @L_80052774
    @L_80052070:
    cmpwi r3, 0x60a
    beq @L_80052C84
    bge @L_80052CC4
    b @L_80052C04
    @L_80052080:
    cmpwi r3, 0xfc7
    beq @L_80052EC4
    bge @L_800520A4
    cmpwi r3, 0x60f
    beq @L_80052E44
    bge @L_80051EC4
    cmpwi r3, 0x60e
    bge @L_80052E04
    b @L_80052D84
    @L_800520A4:
    cmpwi r3, 0x1197
    beq @L_80052FB4
    bge @L_80051EC4
    cmpwi r3, 0xfc9
    bge @L_80051EC4
    b @L_80052F3C
    @L_800520BC:
    mr r4, r31
    li r3, 0x0
    li r5, 0x1f
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052110
    mr r4, r31
    li r3, 0x0
    li r5, 0x1f
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052110:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x1f
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052134:
    mr r4, r31
    li r3, 0x0
    li r5, 0x20
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052188
    mr r4, r31
    li r3, 0x0
    li r5, 0x20
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052188:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x20
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_800521AC:
    mr r4, r31
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052200
    mr r4, r31
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052200:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x23
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052224:
    mr r4, r31
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052278
    mr r4, r31
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052278:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_8005229C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x25
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x38
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80052314
    lwz r0, 0x38(r1)
    cmpwi r0, 0x64
    ble @L_800522E8
    li r0, 0x64
    stw r0, 0x38(r1)
    @L_800522E8:
    lwz r0, 0x38(r1)
    cmpwi r0, 0x0
    bge @L_800522FC
    li r0, 0x0
    stw r0, 0x38(r1)
    @L_800522FC:
    lwz r7, 0x38(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x25
    li r6, 0x0
    bl fn_801FAA58
    @L_80052314:
    bl fn_8001E200
    b @L_80051EC4
    @L_8005231C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x34
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80052394
    lwz r0, 0x34(r1)
    cmpwi r0, 0x64
    ble @L_80052368
    li r0, 0x64
    stw r0, 0x34(r1)
    @L_80052368:
    lwz r0, 0x34(r1)
    cmpwi r0, 0x0
    bge @L_8005237C
    li r0, 0x0
    stw r0, 0x34(r1)
    @L_8005237C:
    lwz r7, 0x34(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_801FAA58
    @L_80052394:
    bl fn_8001E200
    b @L_80051EC4
    @L_8005239C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x30
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80052414
    lwz r0, 0x30(r1)
    cmpwi r0, 0x64
    ble @L_800523E8
    li r0, 0x64
    stw r0, 0x30(r1)
    @L_800523E8:
    lwz r0, 0x30(r1)
    cmpwi r0, 0x0
    bge @L_800523FC
    li r0, 0x0
    stw r0, 0x30(r1)
    @L_800523FC:
    lwz r7, 0x30(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_801FAA58
    @L_80052414:
    bl fn_8001E200
    b @L_80051EC4
    @L_8005241C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x28
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052470
    mr r4, r31
    li r3, 0x0
    li r5, 0x28
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052470:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x28
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052494:
    mr r4, r31
    li r3, 0x0
    li r5, 0x29
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800524E8
    mr r4, r31
    li r3, 0x0
    li r5, 0x29
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_800524E8:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x29
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_8005250C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052560
    mr r4, r31
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052560:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052584:
    mr r4, r31
    li r3, 0x0
    li r5, 0x2b
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800525D8
    mr r4, r31
    li r3, 0x0
    li r5, 0x2b
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_800525D8:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x2b
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_800525FC:
    mr r4, r31
    li r3, 0x0
    li r5, 0x2c
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x2c
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80052674
    lwz r0, 0x2c(r1)
    cmpwi r0, 0x64
    ble @L_80052648
    li r0, 0x64
    stw r0, 0x2c(r1)
    @L_80052648:
    lwz r0, 0x2c(r1)
    cmpwi r0, 0x0
    bge @L_8005265C
    li r0, 0x0
    stw r0, 0x2c(r1)
    @L_8005265C:
    lwz r7, 0x2c(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x2c
    li r6, 0x0
    bl fn_801FAA58
    @L_80052674:
    bl fn_8001E200
    b @L_80051EC4
    @L_8005267C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800526D0
    mr r4, r31
    li r3, 0x0
    li r5, 0x2d
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_800526D0:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x2d
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_800526F4:
    mr r4, r31
    li r3, 0x0
    li r5, 0x2e
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x28
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8005276C
    lwz r0, 0x28(r1)
    cmpwi r0, 0x54
    ble @L_80052740
    li r0, 0x54
    stw r0, 0x28(r1)
    @L_80052740:
    lwz r0, 0x28(r1)
    cmpwi r0, 0x0
    bge @L_80052754
    li r0, 0x0
    stw r0, 0x28(r1)
    @L_80052754:
    lwz r7, 0x28(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x2e
    li r6, 0x0
    bl fn_801FAA58
    @L_8005276C:
    bl fn_8001E200
    b @L_80051EC4
    @L_80052774:
    mr r4, r31
    li r3, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x24
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_800527EC
    lwz r0, 0x24(r1)
    cmpwi r0, 0x54
    ble @L_800527C0
    li r0, 0x54
    stw r0, 0x24(r1)
    @L_800527C0:
    lwz r0, 0x24(r1)
    cmpwi r0, 0x0
    bge @L_800527D4
    li r0, 0x0
    stw r0, 0x24(r1)
    @L_800527D4:
    lwz r7, 0x24(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_801FAA58
    @L_800527EC:
    bl fn_8001E200
    b @L_80051EC4
    @L_800527F4:
    mr r4, r31
    li r3, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052848
    mr r4, r31
    li r3, 0x0
    li r5, 0x30
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052848:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x30
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_8005286C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x31
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800528C0
    mr r4, r31
    li r3, 0x0
    li r5, 0x31
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_800528C0:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x31
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_800528E4:
    mr r4, r31
    li r3, 0x0
    li r5, 0x32
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052938
    mr r4, r31
    li r3, 0x0
    li r5, 0x32
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052938:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x32
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_8005295C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x33
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800529B0
    mr r4, r31
    li r3, 0x0
    li r5, 0x33
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_800529B0:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x33
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_800529D4:
    mr r4, r31
    li r3, 0x0
    li r5, 0x34
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052A28
    mr r4, r31
    li r3, 0x0
    li r5, 0x34
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052A28:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x34
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052A4C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x35
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x20
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80052AC4
    lwz r0, 0x20(r1)
    cmpwi r0, 0x64
    ble @L_80052A98
    li r0, 0x64
    stw r0, 0x20(r1)
    @L_80052A98:
    lwz r0, 0x20(r1)
    cmpwi r0, 0x0
    bge @L_80052AAC
    li r0, 0x0
    stw r0, 0x20(r1)
    @L_80052AAC:
    lwz r7, 0x20(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x35
    li r6, 0x0
    bl fn_801FAA58
    @L_80052AC4:
    bl fn_8001E200
    b @L_80051EC4
    @L_80052ACC:
    mr r4, r31
    li r3, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052B20
    mr r4, r31
    li r3, 0x0
    li r5, 0x36
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052B20:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x36
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052B44:
    mr r4, r31
    li r3, 0x0
    li r5, 0x37
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x1c
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80052BBC
    lwz r0, 0x1c(r1)
    cmpwi r0, 0x64
    ble @L_80052B90
    li r0, 0x64
    stw r0, 0x1c(r1)
    @L_80052B90:
    lwz r0, 0x1c(r1)
    cmpwi r0, 0x0
    bge @L_80052BA4
    li r0, 0x0
    stw r0, 0x1c(r1)
    @L_80052BA4:
    lwz r7, 0x1c(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x37
    li r6, 0x0
    bl fn_801FAA58
    @L_80052BBC:
    bl fn_8001E200
    b @L_80051EC4
    @L_80052BC4:
    mr r4, r31
    li r3, 0x0
    li r5, 0x39
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r3, r3, 16
    bl fn_80008518
    cmpwi r3, 0x0
    blt @L_80051EC4
    mr r4, r31
    mr r7, r3
    li r3, 0x0
    li r5, 0x39
    li r6, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052C04:
    mr r4, r31
    li r3, 0x0
    li r5, 0x3a
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x18
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80052C7C
    lwz r0, 0x18(r1)
    cmpwi r0, 0xff
    ble @L_80052C50
    li r0, 0xff
    stw r0, 0x18(r1)
    @L_80052C50:
    lwz r0, 0x18(r1)
    cmpwi r0, 0x0
    bge @L_80052C64
    li r0, 0x0
    stw r0, 0x18(r1)
    @L_80052C64:
    lwz r7, 0x18(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x3a
    li r6, 0x0
    bl fn_801FAA58
    @L_80052C7C:
    bl fn_8001E200
    b @L_80051EC4
    @L_80052C84:
    mr r4, r31
    li r3, 0x0
    li r5, 0x39
    li r6, 0x1
    bl fn_801FB1C0
    clrlwi r3, r3, 16
    bl fn_80008518
    cmpwi r3, 0x0
    blt @L_80051EC4
    mr r4, r31
    mr r7, r3
    li r3, 0x0
    li r5, 0x39
    li r6, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052CC4:
    mr r4, r31
    li r3, 0x0
    li r5, 0x3a
    li r6, 0x1
    bl fn_801FB1C0
    addi r4, r1, 0x14
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80052D3C
    lwz r0, 0x14(r1)
    cmpwi r0, 0xff
    ble @L_80052D10
    li r0, 0xff
    stw r0, 0x14(r1)
    @L_80052D10:
    lwz r0, 0x14(r1)
    cmpwi r0, 0x0
    bge @L_80052D24
    li r0, 0x0
    stw r0, 0x14(r1)
    @L_80052D24:
    lwz r7, 0x14(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x3a
    li r6, 0x1
    bl fn_801FAA58
    @L_80052D3C:
    bl fn_8001E200
    b @L_80051EC4
    @L_80052D44:
    mr r4, r31
    li r3, 0x0
    li r5, 0x3b
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r3, r3, 24
    bl fn_80008460
    cmpwi r3, 0x0
    blt @L_80051EC4
    mr r4, r31
    mr r7, r3
    li r3, 0x0
    li r5, 0x3b
    li r6, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052D84:
    mr r4, r31
    li r3, 0x0
    li r5, 0x3c
    li r6, 0x0
    bl fn_801FB1C0
    addi r4, r1, 0x10
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80052DFC
    lwz r0, 0x10(r1)
    cmpwi r0, 0xff
    ble @L_80052DD0
    li r0, 0xff
    stw r0, 0x10(r1)
    @L_80052DD0:
    lwz r0, 0x10(r1)
    cmpwi r0, 0x0
    bge @L_80052DE4
    li r0, 0x0
    stw r0, 0x10(r1)
    @L_80052DE4:
    lwz r7, 0x10(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x3c
    li r6, 0x0
    bl fn_801FAA58
    @L_80052DFC:
    bl fn_8001E200
    b @L_80051EC4
    @L_80052E04:
    mr r4, r31
    li r3, 0x0
    li r5, 0x3b
    li r6, 0x1
    bl fn_801FB1C0
    clrlwi r3, r3, 24
    bl fn_80008460
    cmpwi r3, 0x0
    blt @L_80051EC4
    mr r4, r31
    mr r7, r3
    li r3, 0x0
    li r5, 0x3b
    li r6, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052E44:
    mr r4, r31
    li r3, 0x0
    li r5, 0x3c
    li r6, 0x1
    bl fn_801FB1C0
    addi r4, r1, 0xc
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80052EBC
    lwz r0, 0xc(r1)
    cmpwi r0, 0xff
    ble @L_80052E90
    li r0, 0xff
    stw r0, 0xc(r1)
    @L_80052E90:
    lwz r0, 0xc(r1)
    cmpwi r0, 0x0
    bge @L_80052EA4
    li r0, 0x0
    stw r0, 0xc(r1)
    @L_80052EA4:
    lwz r7, 0xc(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x3c
    li r6, 0x1
    bl fn_801FAA58
    @L_80052EBC:
    bl fn_8001E200
    b @L_80051EC4
    @L_80052EC4:
    mr r4, r31
    li r3, 0x0
    li r5, 0x21
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052F18
    mr r4, r31
    li r3, 0x0
    li r5, 0x21
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052F18:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x21
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052F3C:
    mr r4, r31
    li r3, 0x0
    li r5, 0x22
    li r6, 0x0
    bl fn_801FB1C0
    cntlzw r0, r3
    li r3, 0x7f
    srwi r6, r0, 5
    li r4, -0x1
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80052F90
    mr r4, r31
    li r3, 0x0
    li r5, 0x22
    li r6, 0x0
    li r7, 0x1
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052F90:
    cmpwi r0, 0x1
    bne @L_80051EC4
    mr r4, r31
    li r3, 0x0
    li r5, 0x22
    li r6, 0x0
    li r7, 0x0
    bl fn_801FAA58
    b @L_80051EC4
    @L_80052FB4:
    mr r4, r31
    li r3, 0x0
    li r5, 0x38
    li r6, 0x1
    bl fn_801FB1C0
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8005302C
    lwz r0, 0x8(r1)
    cmpwi r0, 0xff
    ble @L_80053000
    li r0, 0xff
    stw r0, 0x8(r1)
    @L_80053000:
    lwz r0, 0x8(r1)
    cmpwi r0, 0x0
    bge @L_80053014
    li r0, 0x0
    stw r0, 0x8(r1)
    @L_80053014:
    lwz r7, 0x8(r1)
    mr r4, r31
    li r3, 0x0
    li r5, 0x38
    li r6, 0x1
    bl fn_801FAA58
    @L_8005302C:
    bl fn_8001E200
    b @L_80051EC4
    @L_80053034:
    li r3, 0x86
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1
    @L_80053048:
    lwz r0, 0x84(r1)
    lwz r31, 0x7c(r1)
    lwz r30, 0x78(r1)
    lwz r29, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
