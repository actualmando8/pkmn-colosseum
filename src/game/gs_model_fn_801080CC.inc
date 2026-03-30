    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r4
    bl fn_80104704
    mr r31, r3
    cmplwi r31, 0x0
    beq @L_801081D8
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x0
    bne @L_80108118
    li r0, 0x0
    stw r0, 0x24(r31)
    sth r0, 0x28(r31)
    b @L_80108134
@L_80108118:
    addi r3, r31, 0x24
    li r4, 0x0
    li r5, 0x3c
    bl memset
    rlwinm r3, r28, 0, 16, 31
    bl fn_8005D830
    stw r3, 0x24(r31)
@L_80108134:
    lis r29, lbl_80404B8C@ha
    lha r0, 0x84(r31)
    addi r29, r29, lbl_80404B8C@l
    sth r0, 0x0(r29)
    lha r0, 0x86(r31)
    sth r0, 0x2(r29)
    lwz r0, 0x88(r31)
    stw r0, 0x4(r29)
    lbz r0, 0x0(r31)
    stb r0, 0x20(r29)
    lwz r3, 0x4(r31)
    bl fn_8005DA18
    lis r3, lbl_80404B8C@ha
    lha r0, 0x6(r3)
    addi r3, r3, lbl_80404B8C@l
    sth r0, 0x8(r3)
    lwz r3, 0x4(r31)
    bl fn_8005DA18
    lis r30, lbl_80404B8C@ha
    lha r0, 0x8(r3)
    addi r30, r30, lbl_80404B8C@l
    li r28, 0x0
    sth r0, 0xa(r30)
    b @L_801081A4
@L_80108194:
    mr r3, r30
    addi r4, r31, 0x24
    bl fn_801074D4
    addi r28, r28, 0x1
@L_801081A4:
    bl fn_800D3088
    cmplw r28, r3
    blt @L_80108194
    lis r3, lbl_80404B8C@ha
    addi r3, r3, lbl_80404B8C@l
    lha r0, 0x0(r3)
    sth r0, 0x84(r31)
    lha r0, 0x2(r29)
    sth r0, 0x86(r31)
    lwz r0, 0x4(r29)
    stw r0, 0x88(r31)
    lbz r0, 0x20(r29)
    stb r0, 0x0(r31)
@L_801081D8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
