    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_80105624
    mr r31, r3
    bl fn_800573C0
    cmpwi r3, 0x0
    beq @L_800588B4
    li r3, 0x0
    b @L_80058A98
    @L_800588B4:
    bl fn_800566E8
    cmpwi r3, 0x0
    beq @L_800588C8
    li r3, 0x0
    b @L_80058A98
    @L_800588C8:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 24, 25
    cmpwi r0, 0x0
    beq @L_800588F4
    bl fn_80057694
    cmpwi r3, 0x0
    beq @L_800588EC
    li r3, 0x0
    b @L_800588F0
    @L_800588EC:
    li r3, 0x1
    @L_800588F0:
    bl fn_800576A4
    @L_800588F4:
    lhz r0, 0x6(r31)
    rlwinm r0, r0, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_80058924
    lbz r3, 0x95(r30)
    addi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x95(r30)
    cmpwi r0, 0x7
    blt @L_80058924
    li r0, 0x0
    stb r0, 0x95(r30)
    @L_80058924:
    lhz r0, 0x6(r31)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_80058954
    lbz r3, 0x95(r30)
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x95(r30)
    cmpwi r0, 0x0
    bge @L_80058954
    li r0, 0x6
    stb r0, 0x95(r30)
    @L_80058954:
    lhz r0, 0x6(r31)
    cmplwi r0, 0x8
    bne @L_800589C8
    lbz r0, 0x95(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80058978
    cmpwi r0, 0x7
    blt @L_80058980
    @L_80058978:
    li r0, 0x2
    b @L_80058994
    @L_80058980:
    lis r3, lbl_802677D0@ha
    slwi r0, r0, 4
    addi r3, r3, lbl_802677D0@l
    add r3, r3, r0
    lwz r0, 0xc(r3)
    @L_80058994:
    cmpwi r0, 0x1
    beq @L_800589BC
    bge @L_800589C8
    cmpwi r0, 0x0
    bge @L_800589AC
    b @L_800589C8
    @L_800589AC:
    lwz r0, lbl_8047A598(r13)
    extsb r0, r0
    stb r0, 0x95(r30)
    b @L_800589C8
    @L_800589BC:
    li r0, 0x1
    stb r0, 0x98(r30)
    stb r0, 0x99(r30)
    @L_800589C8:
    lhz r0, 0x6(r31)
    cmplwi r0, 0x4
    bne @L_80058A1C
    lbz r0, 0x95(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_800589EC
    cmpwi r0, 0x7
    blt @L_800589F4
    @L_800589EC:
    li r0, 0x2
    b @L_80058A08
    @L_800589F4:
    lis r3, lbl_802677D0@ha
    slwi r0, r0, 4
    addi r3, r3, lbl_802677D0@l
    add r3, r3, r0
    lwz r0, 0xc(r3)
    @L_80058A08:
    cmpwi r0, 0x1
    bne @L_80058A1C
    lwz r0, lbl_8047A59C(r13)
    extsb r0, r0
    stb r0, 0x95(r30)
    @L_80058A1C:
    lbz r3, 0x95(r30)
    lbz r0, 0x97(r30)
    extsb r4, r3
    extsb r0, r0
    cmpw r4, r0
    beq @L_80058A94
    cmpwi r4, 0x0
    blt @L_80058A44
    cmpwi r4, 0x7
    blt @L_80058A4C
    @L_80058A44:
    li r0, 0x2
    b @L_80058A60
    @L_80058A4C:
    lis r3, lbl_802677D0@ha
    slwi r0, r4, 4
    addi r3, r3, lbl_802677D0@l
    add r3, r3, r0
    lwz r0, 0xc(r3)
    @L_80058A60:
    cmpwi r0, 0x0
    bne @L_80058A6C
    stw r4, lbl_8047A59C(r13)
    @L_80058A6C:
    cmpwi r0, 0x0
    beq @L_80058A78
    stw r4, lbl_8047A598(r13)
    @L_80058A78:
    mr r3, r30
    bl fn_80104318
    mr r4, r3
    li r5, 0x0
    lha r3, 0x2(r4)
    lha r4, 0x4(r4)
    bl fn_80057830
    @L_80058A94:
    li r3, 0x0
    @L_80058A98:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
