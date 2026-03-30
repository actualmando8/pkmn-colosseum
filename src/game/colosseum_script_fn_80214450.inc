    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x3
    mr r4, r0
    bl fn_801F025C
    li r4, 0x49
    mr r31, r3
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802144AC
    mr r3, r31
    li r4, 0x48
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80214590
@L_802144AC:
    mr r3, r31
    li r4, 0x49
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80214510
    mr r3, r31
    li r4, 0x48
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80214510
    mr r3, r31
    li r4, 0x49
    bl fn_801F6EEC
    mr r3, r31
    li r4, 0x48
    bl fn_801F6EEC
    lis r3, lbl_80379F58@ha
    li r0, 0x3
    addi r3, r3, lbl_80379F58@l
    addis r3, r3, 0x1
    stb r0, 0x6002(r3)
    stb r0, 0x60a1(r3)
    b @L_802145A8
@L_80214510:
    mr r3, r31
    li r4, 0x49
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80214550
    mr r3, r31
    li r4, 0x49
    bl fn_801F6EEC
    lis r3, lbl_80379F58@ha
    li r0, 0x1
    addi r3, r3, lbl_80379F58@l
    addis r3, r3, 0x1
    stb r0, 0x6002(r3)
    stb r0, 0x60a1(r3)
    b @L_802145A8
@L_80214550:
    mr r3, r31
    li r4, 0x48
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802145A8
    mr r3, r31
    li r4, 0x48
    bl fn_801F6EEC
    lis r3, lbl_80379F58@ha
    li r0, 0x2
    addi r3, r3, lbl_80379F58@l
    addis r3, r3, 0x1
    stb r0, 0x6002(r3)
    stb r0, 0x60a1(r3)
    b @L_802145A8
@L_80214590:
    lis r3, lbl_80379F58@ha
    li r0, 0x0
    addi r3, r3, lbl_80379F58@l
    addis r3, r3, 0x1
    stb r0, 0x6002(r3)
    stb r0, 0x60a1(r3)
@L_802145A8:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r31, 0xc(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
