    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    addi r11, r1, 0x70
    bl _savefpr_27
    stw r31, 0x44(r1)
    stw r30, 0x40(r1)
    stw r29, 0x3c(r1)
    mr. r31, r3
    fmr f27, f1
    fmr f28, f2
    mr r30, r4
    fmr f29, f3
    mr r29, r5
    fmr f30, f4
    fmr f31, f5
    bne @L_80196F4C
    cmpwi r30, 0x0
    bne @L_80196F4C
    cmpwi r29, 0x0
    beq @L_801972F8
@L_80196F4C:
    cmpwi r29, 0x0
    beq @L_80196FE8
    lis r4, lbl_8036C720@ha
    addi r3, r1, 0x10
    addi r4, r4, lbl_8036C720@l
    li r5, 0x4
    li r6, 0x4
    li r7, 0x11
    li r8, 0x1
    li r9, 0x1
    li r10, 0x0
    bl fn_800BA9E4
    addi r3, r1, 0x10
    li r4, 0x0
    bl fn_800BAFFC
    li r3, 0x1
    bl fn_800B884C
    li r3, 0x0
    li r4, 0x1
    li r5, 0x4
    li r6, 0x3c
    li r7, 0x0
    li r8, 0x7d
    bl fn_800B857C
    li r3, 0x1
    bl fn_800BC8C8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x4
    bl fn_800BC6F0
    li r3, 0x0
    li r4, 0x4
    bl fn_800BC114
    li r3, 0x2
    li r4, 0x11
    li r5, 0x0
    bl fn_800BC66C
    b @L_80197018
@L_80196FE8:
    li r3, 0x0
    bl fn_800B884C
    li r3, 0x1
    bl fn_800BC8C8
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0x4
    bl fn_800BC6F0
    li r3, 0x0
    li r4, 0x4
    bl fn_800BC114
@L_80197018:
    li r3, 0x0
    bl fn_800B94F0
    li r3, 0x7
    li r4, 0x0
    li r5, 0x1
    li r6, 0x7
    li r7, 0x0
    bl fn_800BC618
    li r3, 0x1
    bl fn_800BCEBC
    neg r0, r29
    li r3, 0x1
    or r0, r0, r29
    li r4, 0x7
    srwi r5, r0, 31
    bl fn_800BCE88
    li r3, 0x2
    li r4, 0x1
    li r5, 0x0
    li r6, 0x3
    bl fn_800BCDDC
    neg r0, r31
    or r0, r0, r31
    srwi r3, r0, 31
    bl fn_800BCE30
    neg r0, r30
    or r0, r0, r30
    srwi r3, r0, 31
    bl fn_800BCE5C
    li r3, 0x1
    bl fn_800BA6B0
    li r3, 0x4
    li r4, 0x0
    li r5, 0x0
    li r6, 0x1
    li r7, 0x0
    li r8, 0x0
    li r9, 0x2
    bl fn_800BA6F4
    bl fn_800B7D3C
    li r3, 0x0
    li r4, 0x9
    li r5, 0x1
    li r6, 0x4
    li r7, 0x0
    bl fn_800B7D74
    li r3, 0x0
    li r4, 0xb
    li r5, 0x1
    li r6, 0x5
    li r7, 0x0
    bl fn_800B7D74
    li r3, 0x0
    li r4, 0xd
    li r5, 0x1
    li r6, 0x0
    li r7, 0x0
    bl fn_800B7D74
    lis r3, lbl_8036CBC0@ha
    li r4, 0x0
    addi r3, r3, lbl_8036CBC0@l
    bl fn_800BD4B4
    li r3, 0x0
    bl fn_800BD554
    li r3, 0x9
    li r4, 0x1
    bl fn_800B7874
    li r3, 0xb
    li r4, 0x1
    bl fn_800B7874
    li r3, 0xd
    li r4, 0x1
    bl fn_800B7874
    lwz r0, lbl_80478C60(r13)
    li r3, 0x80
    li r4, 0x0
    li r5, 0x4
    stw r0, 0x8(r1)
    lwz r0, 0x8(r1)
    stw r0, 0xc(r1)
    bl fn_800B928C
    lis r3, 0xcc01
    lis r4, 0xcc01
    stfs f29, -0x8000(r3)
    lis r5, 0xcc01
    lbz r0, 0xc(r1)
    lis r3, 0xcc01
    stfs f27, -0x8000(r4)
    lis r31, 0xcc01
    lbz r6, 0xd(r1)
    lis r12, 0xcc01
    stfs f31, -0x8000(r5)
    li r11, 0x1
    lbz r7, 0xe(r1)
    lis r10, 0xcc01
    stb r0, -0x8000(r3)
    lis r3, 0xcc01
    lbz r4, 0xf(r1)
    stb r6, -0x8000(r3)
    lis r3, 0xcc01
    lbz r5, 0xc(r1)
    stb r7, -0x8000(r3)
    lis r3, 0xcc01
    lbz r6, 0xd(r1)
    stb r4, -0x8000(r3)
    li r4, 0x0
    lis r3, 0xcc01
    lbz r8, 0xe(r1)
    stb r4, -0x8000(r3)
    li r4, 0x0
    lis r3, 0xcc01
    lbz r9, 0xf(r1)
    stb r4, -0x8000(r3)
    lis r3, 0xcc01
    lis r4, 0xcc01
    lbz r7, 0xc(r1)
    stfs f30, -0x8000(r3)
    lis r3, 0xcc01
    lbz r0, 0xd(r1)
    stfs f27, -0x8000(r3)
    lis r3, 0xcc01
    lbz r30, 0xf(r1)
    stfs f31, -0x8000(r3)
    lbz r3, 0xe(r1)
    stb r5, -0x8000(r4)
    lis r4, 0xcc01
    lis r5, 0xcc01
    lbz r29, 0xf(r1)
    stb r6, -0x8000(r4)
    li r6, 0x1
    stb r8, -0x8000(r5)
    lis r5, 0xcc01
    li r8, 0x0
    stb r9, -0x8000(r5)
    lis r5, 0xcc01
    lis r9, 0xcc01
    stb r6, -0x8000(r5)
    lis r6, 0xcc01
    lis r5, 0xcc01
    stb r8, -0x8000(r6)
    lis r8, 0xcc01
    lis r6, 0xcc01
    stfs f30, -0x8000(r5)
    lis r5, 0xcc01
    stfs f28, -0x8000(r9)
    li r9, 0x1
    stfs f31, -0x8000(r8)
    lis r8, 0xcc01
    stb r7, -0x8000(r6)
    lis r7, 0xcc01
    lis r6, 0xcc01
    stb r0, -0x8000(r5)
    lis r5, 0xcc01
    stb r3, -0x8000(r31)
    stb r30, -0x8000(r12)
    stb r11, -0x8000(r10)
    stb r9, -0x8000(r8)
    stfs f29, -0x8000(r7)
    stfs f28, -0x8000(r6)
    stfs f31, -0x8000(r5)
    lbz r0, 0xc(r1)
    lis r4, 0xcc01
    lbz r5, 0xd(r1)
    lis r3, 0xcc01
    stb r0, -0x8000(r4)
    lis r4, 0xcc01
    lbz r6, 0xe(r1)
    stb r5, -0x8000(r3)
    lis r3, 0xcc01
    clrlwi r0, r6, 24
    li r5, 0x0
    stb r0, -0x8000(r3)
    lis r3, 0xcc01
    li r0, 0x1
    lis r6, 0xcc01
    stb r29, -0x8000(r3)
    li r3, 0x0
    stb r5, -0x8000(r4)
    li r4, 0x11
    li r5, 0x0
    stb r0, -0x8000(r6)
    bl fn_800BC66C
    li r3, -0x1
    bl fn_801B25C4
@L_801972F8:
    addi r11, r1, 0x70
    bl _restfpr_27
    lwz r0, 0x74(r1)
    lwz r31, 0x44(r1)
    lwz r30, 0x40(r1)
    lwz r29, 0x3c(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
