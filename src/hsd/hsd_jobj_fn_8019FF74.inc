    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    stw r30, 0x18(r1)
    mr. r30, r3
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    beq @L_801A013C
    cmplwi r31, 0x0
    bne @L_8019FFA8
    b @L_801A013C
    @L_8019FFA8:
    cmplwi r31, 0x0
    beq @L_801A00DC
    lwz r0, 0xc(r31)
    lwz r29, 0x8(r31)
    cmplwi r0, 0x0
    beq @L_801A00D4
    lwz r3, 0xc(r31)
    lwz r0, 0x10(r3)
    cmplw r0, r31
    bne @L_8019FFDC
    lwz r3, 0xc(r31)
    stw r29, 0x10(r3)
    b @L_801A006C
    @L_8019FFDC:
    cmplwi r31, 0x0
    beq @L_8019FFF0
    lwz r0, 0xc(r31)
    cmplwi r0, 0x0
    bne @L_8019FFF8
    @L_8019FFF0:
    li r28, 0x0
    b @L_801A0050
    @L_8019FFF8:
    lwz r3, 0xc(r31)
    lwz r0, 0x10(r3)
    cmplw r31, r0
    bne @L_801A0010
    li r28, 0x0
    b @L_801A0050
    @L_801A0010:
    lwz r3, 0xc(r31)
    lwz r28, 0x10(r3)
    b @L_801A0030
    @L_801A001C:
    lwz r0, 0x8(r28)
    cmplw r0, r31
    bne @L_801A002C
    b @L_801A0050
    @L_801A002C:
    lwz r28, 0x8(r28)
    @L_801A0030:
    cmplwi r28, 0x0
    bne @L_801A001C
    lis r4, lbl_80274B28@ha
    la r3, lbl_8047DB20(r2)
    addi r5, r4, lbl_80274B28@l
    li r4, 0x5f8
    bl fn_80196D78
    li r28, 0x0
    @L_801A0050:
    cmplwi r28, 0x0
    bne @L_801A0068
    la r3, lbl_8047DB20(r2)
    li r4, 0x57b
    la r5, lbl_8047DB28(r2)
    bl fn_80196E10
    @L_801A0068:
    stw r29, 0x8(r28)
    @L_801A006C:
    lwz r6, 0xc(r31)
    b @L_801A00C4
    @L_801A0074:
    lis r4, 0x9000
    lwz r5, 0x10(r6)
    subi r4, r4, 0x1
    b @L_801A00A0
    @L_801A0084:
    lwz r0, 0x14(r5)
    lwz r3, 0x14(r5)
    slwi r0, r0, 10
    lwz r5, 0x8(r5)
    or r0, r3, r0
    rlwinm r0, r0, 0, 1, 3
    or r4, r4, r0
    @L_801A00A0:
    cmplwi r5, 0x0
    bne @L_801A0084
    lwz r0, 0x14(r6)
    andc. r0, r0, r4
    beq @L_801A00CC
    lwz r0, 0x14(r6)
    and r0, r0, r4
    stw r0, 0x14(r6)
    lwz r6, 0x8(r6)
    @L_801A00C4:
    cmplwi r6, 0x0
    bne @L_801A0074
    @L_801A00CC:
    li r0, 0x0
    stw r0, 0xc(r31)
    @L_801A00D4:
    li r0, 0x0
    stw r0, 0x8(r31)
    @L_801A00DC:
    lwz r0, 0xc(r30)
    stw r0, 0xc(r31)
    lwz r0, 0x8(r30)
    stw r0, 0x8(r31)
    stw r31, 0x8(r30)
    lwz r0, 0xc(r30)
    cmplwi r0, 0x0
    beq @L_801A013C
    lwz r0, 0x14(r31)
    lwz r3, 0x14(r31)
    slwi r0, r0, 10
    lwz r4, 0xc(r30)
    or r0, r3, r0
    rlwinm r3, r0, 0, 1, 3
    b @L_801A0134
    @L_801A0118:
    lwz r0, 0x14(r4)
    andc. r0, r3, r0
    beq @L_801A013C
    lwz r0, 0x14(r4)
    or r0, r0, r3
    stw r0, 0x14(r4)
    lwz r4, 0xc(r4)
    @L_801A0134:
    cmplwi r4, 0x0
    bne @L_801A0118
    @L_801A013C:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
