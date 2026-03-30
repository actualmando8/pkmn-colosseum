    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r29, r3
    mr r26, r4
    rlwinm r31, r26, 0, 30, 30
    rlwinm r28, r26, 0, 29, 29
    cmplwi r31, 0x0
    li r30, 0x0
    bne @L_801048E0
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lwz r27, 0xc(r3)
    b @L_801048D8
@L_80104864:
    cmplw r27, r29
    mr r3, r27
    bne @L_80104890
    li r0, 0x0
    b @L_8010489C
    b @L_80104890
@L_8010487C:
    lwz r3, 0xc(r3)
    cmplw r3, r29
    bne @L_80104890
    li r0, 0x1
    b @L_8010489C
@L_80104890:
    cmplwi r3, 0x0
    bne @L_8010487C
    li r0, 0x0
@L_8010489C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801048D4
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_801048CC
    lwz r3, 0x4(r27)
    bl fn_8005DA18
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 29, 31, 31
    cmplwi r0, 0x0
    bne @L_801048D4
@L_801048CC:
    li r0, 0x1
    stb r0, 0xa(r27)
@L_801048D4:
    lwz r27, 0x10(r27)
@L_801048D8:
    cmplwi r27, 0x0
    bne @L_80104864
@L_801048E0:
    rlwinm r0, r26, 0, 31, 31
    cmplwi r0, 0x0
    bne @L_80104958
    cmplwi r29, 0x0
    beq @L_80104958
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80104918
    lwz r3, 0x4(r29)
    bl fn_8005DA18
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 29, 31, 31
    cmplwi r0, 0x0
    bne @L_80104920
@L_80104918:
    li r0, 0x1
    stb r0, 0xa(r29)
@L_80104920:
    cmplwi r31, 0x0
    beq @L_80104958
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lwz r3, 0xc(r3)
    b @L_80104950
@L_80104938:
    lwz r0, 0xc(r3)
    cmplw r0, r29
    bne @L_8010494C
    lwz r0, 0xc(r29)
    stw r0, 0xc(r3)
@L_8010494C:
    lwz r3, 0x10(r3)
@L_80104950:
    cmplwi r3, 0x0
    bne @L_80104938
@L_80104958:
    lis r4, lbl_80404ACC@ha
    addi r4, r4, lbl_80404ACC@l
    lwz r3, 0x4(r4)
    cmpwi r3, 0x0
    bgt @L_80104974
    li r5, 0x0
    b @L_8010499C
@L_80104974:
    lwz r5, 0xc(r4)
    b @L_80104990
@L_8010497C:
    lwz r0, 0x4(r5)
    cmpw r0, r3
    bne @L_8010498C
    b @L_8010499C
@L_8010498C:
    lwz r5, 0x10(r5)
@L_80104990:
    cmplwi r5, 0x0
    bne @L_8010497C
    li r5, 0x0
@L_8010499C:
    lis r3, lbl_80404ACC@ha
    mr r29, r5
    addi r3, r3, lbl_80404ACC@l
    b @L_80104A64
@L_801049AC:
    lbz r0, 0xa(r29)
    cmplwi r0, 0x0
    bne @L_801049C4
    lbz r0, 0x18(r29)
    cmplwi r0, 0x0
    beq @L_80104A54
@L_801049C4:
    lwz r6, 0xc(r3)
    b @L_80104A3C
@L_801049CC:
    lbz r0, 0xa(r6)
    cmplwi r0, 0x0
    bne @L_80104A38
    lbz r0, 0x18(r6)
    cmplwi r0, 0x0
    bne @L_80104A38
    lwz r0, 0xc(r29)
    mr r5, r6
    cmplw r6, r0
    bne @L_80104A14
    li r0, 0x0
    b @L_80104A20
    b @L_80104A14
@L_80104A00:
    lwz r5, 0xc(r5)
    cmplw r5, r0
    bne @L_80104A14
    li r0, 0x1
    b @L_80104A20
@L_80104A14:
    cmplwi r5, 0x0
    bne @L_80104A00
    li r0, 0x0
@L_80104A20:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80104A38
    lwz r0, 0x4(r6)
    li r30, 0x1
    stw r0, 0x4(r4)
@L_80104A38:
    lwz r6, 0x10(r6)
@L_80104A3C:
    cmplwi r6, 0x0
    bne @L_801049CC
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_80104A6C
    b @L_80104A60
@L_80104A54:
    lwz r0, 0x4(r29)
    stw r0, 0x4(r4)
    b @L_80104A6C
@L_80104A60:
    lwz r29, 0xc(r29)
@L_80104A64:
    cmplwi r29, 0x0
    bne @L_801049AC
@L_80104A6C:
    cmplwi r29, 0x0
    bne @L_80104A7C
    li r0, 0x0
    stw r0, 0x4(r4)
@L_80104A7C:
    li r3, 0x0
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
