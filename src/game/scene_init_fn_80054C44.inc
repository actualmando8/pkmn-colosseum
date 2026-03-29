    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r31, r3
    bl fn_80105624
    mr r30, r3
    bl fn_800573C0
    cmpwi r3, 0x0
    beq @L_80054C78
    li r3, 0x0
    b @L_80054E64
    @L_80054C78:
    bl fn_800566E8
    cmpwi r3, 0x0
    beq @L_80054C8C
    li r3, 0x0
    b @L_80054E64
    @L_80054C8C:
    lhz r0, 0x6(r30)
    rlwinm r0, r0, 0, 24, 25
    cmpwi r0, 0x0
    beq @L_80054CB8
    bl fn_80057694
    cmpwi r3, 0x0
    beq @L_80054CB0
    li r3, 0x0
    b @L_80054CB4
    @L_80054CB0:
    li r3, 0x1
    @L_80054CB4:
    bl fn_800576A4
    @L_80054CB8:
    lhz r4, 0x0(r30)
    rlwinm r0, r4, 0, 21, 21
    cmpwi r0, 0x0
    beq @L_80054CD4
    bl fn_800567AC
    li r3, 0x0
    b @L_80054E64
    @L_80054CD4:
    rlwinm r0, r4, 0, 22, 22
    cmpwi r0, 0x0
    beq @L_80054CEC
    bl fn_80056704
    li r3, 0x0
    b @L_80054E64
    @L_80054CEC:
    lbz r0, 0x95(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80054D04
    cmpwi r0, 0x20
    blt @L_80054D0C
    @L_80054D04:
    li r0, 0x3
    b @L_80054D20
    @L_80054D0C:
    mulli r0, r0, 0xc
    lis r3, lbl_80267398@ha
    addi r3, r3, lbl_80267398@l
    add r3, r3, r0
    lwz r0, 0x4(r3)
    @L_80054D20:
    cmpwi r0, 0x1
    bne @L_80054D50
    cmplwi r4, 0x8
    bne @L_80054D3C
    bl fn_800567AC
    li r3, 0x0
    b @L_80054E64
    @L_80054D3C:
    cmplwi r4, 0x4
    bne @L_80054D50
    bl fn_80056704
    li r3, 0x0
    b @L_80054E64
    @L_80054D50:
    cmpwi r0, 0x2
    bne @L_80054D84
    lhz r3, 0x6(r30)
    andi. r0, r3, 0x5
    cmpwi r0, 0x0
    beq @L_80054D70
    li r3, 0x0
    b @L_80054E64
    @L_80054D70:
    andi. r0, r3, 0xa
    cmpwi r0, 0x0
    beq @L_80054D84
    li r0, 0x1e
    stb r0, 0x95(r31)
    @L_80054D84:
    lbz r3, 0x95(r31)
    lbz r0, 0x97(r31)
    extsb r3, r3
    extsb r0, r0
    cmpw r3, r0
    bne @L_80054E24
    mr r3, r31
    bl fn_80102F38
    lbz r0, 0x97(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80054DBC
    cmpwi r0, 0x20
    blt @L_80054DC4
    @L_80054DBC:
    li r4, 0x3
    b @L_80054DD8
    @L_80054DC4:
    mulli r0, r0, 0xc
    lis r3, lbl_80267398@ha
    addi r3, r3, lbl_80267398@l
    add r3, r3, r0
    lwz r4, 0x4(r3)
    @L_80054DD8:
    lbz r0, 0x95(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80054DF0
    cmpwi r0, 0x20
    blt @L_80054DF8
    @L_80054DF0:
    li r0, 0x3
    b @L_80054E0C
    @L_80054DF8:
    mulli r0, r0, 0xc
    lis r3, lbl_80267398@ha
    addi r3, r3, lbl_80267398@l
    add r3, r3, r0
    lwz r0, 0x4(r3)
    @L_80054E0C:
    cmpwi r4, 0x0
    bne @L_80054E24
    cmpwi r0, 0x2
    bne @L_80054E24
    li r0, 0x1e
    stb r0, 0x95(r31)
    @L_80054E24:
    lbz r3, 0x95(r31)
    lbz r0, 0x97(r31)
    extsb r3, r3
    extsb r0, r0
    cmpw r3, r0
    beq @L_80054E60
    mr r3, r31
    bl fn_80104318
    mr r4, r3
    li r5, 0x0
    lha r3, 0x2(r4)
    lha r4, 0x4(r4)
    bl fn_80057830
    li r3, 0x0
    b @L_80054E64
    @L_80054E60:
    li r3, 0x0
    @L_80054E64:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
