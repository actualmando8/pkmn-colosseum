    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr. r27, r3
    bne @L_801B752C
    li r0, 0x0
    b @L_801B7558
@L_801B752C:
    addis r0, r27, 0x1
    cmplwi r0, 0xffff
    bne @L_801B7540
    li r0, 0x2
    b @L_801B7558
@L_801B7540:
    addis r0, r27, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B7554
    li r0, 0x3
    b @L_801B7558
@L_801B7554:
    lwz r0, 0x0(r27)
@L_801B7558:
    cmpwi r0, 0x4
    beq @L_801B7BA8
    bge @L_801B7BC0
    cmpwi r0, 0x1
    beq @L_801B7570
    b @L_801B7BC0
@L_801B7570:
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B7598
    lwz r0, 0x8(r27)
    cmpwi r0, 0x0
    beq @L_801B75B0
    lwz r3, 0x8(r27)
    subi r0, r3, 0x1
    stw r0, 0x8(r27)
    b @L_801B75B0
@L_801B7598:
    lwz r0, 0x14(r27)
    cmpwi r0, 0x0
    beq @L_801B75B0
    lwz r3, 0x14(r27)
    subi r0, r3, 0x1
    stw r0, 0x14(r27)
@L_801B75B0:
    lwz r0, 0x8(r27)
    cmpwi r0, 0x0
    bne @L_801B7BC0
    lwz r0, 0x14(r27)
    cmpwi r0, 0x0
    bne @L_801B7BC0
    li r28, 0x0
    b @L_801B7B9C
@L_801B75D0:
    rlwinm r0, r28, 3, 0, 28
    rlwinm r4, r28, 3, 0, 28
    add r3, r27, r0
    lwz r30, 0x38(r3)
    add r3, r27, r4
    lbz r3, 0x35(r3)
    cmplwi r30, 0x0
    bne @L_801B75F8
    li r0, 0x0
    b @L_801B7624
@L_801B75F8:
    addis r0, r30, 0x1
    cmplwi r0, 0xffff
    bne @L_801B760C
    li r0, 0x2
    b @L_801B7624
@L_801B760C:
    addis r0, r30, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B7620
    li r0, 0x3
    b @L_801B7624
@L_801B7620:
    lwz r0, 0x0(r30)
@L_801B7624:
    cmpwi r0, 0x4
    beq @L_801B789C
    bge @L_801B78B4
    cmpwi r0, 0x1
    beq @L_801B763C
    b @L_801B78B4
@L_801B763C:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B7664
    lwz r0, 0x8(r30)
    cmpwi r0, 0x0
    beq @L_801B767C
    lwz r3, 0x8(r30)
    subi r0, r3, 0x1
    stw r0, 0x8(r30)
    b @L_801B767C
@L_801B7664:
    lwz r0, 0x14(r30)
    cmpwi r0, 0x0
    beq @L_801B767C
    lwz r3, 0x14(r30)
    subi r0, r3, 0x1
    stw r0, 0x14(r30)
@L_801B767C:
    lwz r0, 0x8(r30)
    cmpwi r0, 0x0
    bne @L_801B78B4
    lwz r0, 0x14(r30)
    cmpwi r0, 0x0
    bne @L_801B78B4
    li r31, 0x0
    b @L_801B7890
@L_801B769C:
    rlwinm r0, r31, 3, 0, 28
    rlwinm r4, r31, 3, 0, 28
    add r3, r30, r0
    lwz r29, 0x38(r3)
    add r3, r30, r4
    lbz r26, 0x35(r3)
    mr r3, r29
    bl fn_801B7C60
    cmpwi r3, 0x4
    beq @L_801B777C
    bge @L_801B7794
    cmpwi r3, 0x1
    beq @L_801B76D4
    b @L_801B7794
@L_801B76D4:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B76FC
    lwz r0, 0x8(r29)
    cmpwi r0, 0x0
    beq @L_801B7714
    lwz r3, 0x8(r29)
    subi r0, r3, 0x1
    stw r0, 0x8(r29)
    b @L_801B7714
@L_801B76FC:
    lwz r0, 0x14(r29)
    cmpwi r0, 0x0
    beq @L_801B7714
    lwz r3, 0x14(r29)
    subi r0, r3, 0x1
    stw r0, 0x14(r29)
@L_801B7714:
    lwz r0, 0x8(r29)
    cmpwi r0, 0x0
    bne @L_801B7794
    lwz r0, 0x14(r29)
    cmpwi r0, 0x0
    bne @L_801B7794
    li r26, 0x0
    b @L_801B7770
@L_801B7734:
    rlwinm r3, r26, 3, 0, 28
    rlwinm r0, r26, 3, 0, 28
    add r3, r29, r3
    add r4, r29, r0
    lwz r3, 0x38(r3)
    lbz r4, 0x35(r4)
    bl fn_801B750C
    rlwinm r3, r26, 3, 0, 28
    rlwinm r0, r26, 3, 0, 28
    add r3, r29, r3
    add r4, r29, r0
    lwz r3, 0x58(r3)
    lbz r4, 0x55(r4)
    bl fn_801B750C
    addi r26, r26, 0x1
@L_801B7770:
    cmpwi r26, 0x4
    blt @L_801B7734
    b @L_801B7794
@L_801B777C:
    lbz r0, 0x10(r29)
    cmplwi r0, 0x0
    beq @L_801B7794
    lbz r3, 0x10(r29)
    subi r0, r3, 0x1
    stb r0, 0x10(r29)
@L_801B7794:
    rlwinm r0, r31, 3, 0, 28
    rlwinm r4, r31, 3, 0, 28
    add r3, r30, r0
    lwz r29, 0x58(r3)
    add r3, r30, r4
    lbz r26, 0x55(r3)
    mr r3, r29
    bl fn_801B7C60
    cmpwi r3, 0x4
    beq @L_801B7874
    bge @L_801B788C
    cmpwi r3, 0x1
    beq @L_801B77CC
    b @L_801B788C
@L_801B77CC:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B77F4
    lwz r0, 0x8(r29)
    cmpwi r0, 0x0
    beq @L_801B780C
    lwz r3, 0x8(r29)
    subi r0, r3, 0x1
    stw r0, 0x8(r29)
    b @L_801B780C
@L_801B77F4:
    lwz r0, 0x14(r29)
    cmpwi r0, 0x0
    beq @L_801B780C
    lwz r3, 0x14(r29)
    subi r0, r3, 0x1
    stw r0, 0x14(r29)
@L_801B780C:
    lwz r0, 0x8(r29)
    cmpwi r0, 0x0
    bne @L_801B788C
    lwz r0, 0x14(r29)
    cmpwi r0, 0x0
    bne @L_801B788C
    li r26, 0x0
    b @L_801B7868
@L_801B782C:
    rlwinm r3, r26, 3, 0, 28
    rlwinm r0, r26, 3, 0, 28
    add r3, r29, r3
    add r4, r29, r0
    lwz r3, 0x38(r3)
    lbz r4, 0x35(r4)
    bl fn_801B750C
    rlwinm r3, r26, 3, 0, 28
    rlwinm r0, r26, 3, 0, 28
    add r3, r29, r3
    add r4, r29, r0
    lwz r3, 0x58(r3)
    lbz r4, 0x55(r4)
    bl fn_801B750C
    addi r26, r26, 0x1
@L_801B7868:
    cmpwi r26, 0x4
    blt @L_801B782C
    b @L_801B788C
@L_801B7874:
    lbz r0, 0x10(r29)
    cmplwi r0, 0x0
    beq @L_801B788C
    lbz r3, 0x10(r29)
    subi r0, r3, 0x1
    stb r0, 0x10(r29)
@L_801B788C:
    addi r31, r31, 0x1
@L_801B7890:
    cmpwi r31, 0x4
    blt @L_801B769C
    b @L_801B78B4
@L_801B789C:
    lbz r0, 0x10(r30)
    cmplwi r0, 0x0
    beq @L_801B78B4
    lbz r3, 0x10(r30)
    subi r0, r3, 0x1
    stb r0, 0x10(r30)
@L_801B78B4:
    rlwinm r0, r28, 3, 0, 28
    rlwinm r4, r28, 3, 0, 28
    add r3, r27, r0
    lwz r31, 0x58(r3)
    add r3, r27, r4
    lbz r3, 0x55(r3)
    cmplwi r31, 0x0
    bne @L_801B78DC
    li r0, 0x0
    b @L_801B7908
@L_801B78DC:
    addis r0, r31, 0x1
    cmplwi r0, 0xffff
    bne @L_801B78F0
    li r0, 0x2
    b @L_801B7908
@L_801B78F0:
    addis r0, r31, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B7904
    li r0, 0x3
    b @L_801B7908
@L_801B7904:
    lwz r0, 0x0(r31)
@L_801B7908:
    cmpwi r0, 0x4
    beq @L_801B7B80
    bge @L_801B7B98
    cmpwi r0, 0x1
    beq @L_801B7920
    b @L_801B7B98
@L_801B7920:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B7948
    lwz r0, 0x8(r31)
    cmpwi r0, 0x0
    beq @L_801B7960
    lwz r3, 0x8(r31)
    subi r0, r3, 0x1
    stw r0, 0x8(r31)
    b @L_801B7960
@L_801B7948:
    lwz r0, 0x14(r31)
    cmpwi r0, 0x0
    beq @L_801B7960
    lwz r3, 0x14(r31)
    subi r0, r3, 0x1
    stw r0, 0x14(r31)
@L_801B7960:
    lwz r0, 0x8(r31)
    cmpwi r0, 0x0
    bne @L_801B7B98
    lwz r0, 0x14(r31)
    cmpwi r0, 0x0
    bne @L_801B7B98
    li r30, 0x0
    b @L_801B7B74
@L_801B7980:
    rlwinm r0, r30, 3, 0, 28
    rlwinm r4, r30, 3, 0, 28
    add r3, r31, r0
    lwz r29, 0x38(r3)
    add r3, r31, r4
    lbz r26, 0x35(r3)
    mr r3, r29
    bl fn_801B7C60
    cmpwi r3, 0x4
    beq @L_801B7A60
    bge @L_801B7A78
    cmpwi r3, 0x1
    beq @L_801B79B8
    b @L_801B7A78
@L_801B79B8:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B79E0
    lwz r0, 0x8(r29)
    cmpwi r0, 0x0
    beq @L_801B79F8
    lwz r3, 0x8(r29)
    subi r0, r3, 0x1
    stw r0, 0x8(r29)
    b @L_801B79F8
@L_801B79E0:
    lwz r0, 0x14(r29)
    cmpwi r0, 0x0
    beq @L_801B79F8
    lwz r3, 0x14(r29)
    subi r0, r3, 0x1
    stw r0, 0x14(r29)
@L_801B79F8:
    lwz r0, 0x8(r29)
    cmpwi r0, 0x0
    bne @L_801B7A78
    lwz r0, 0x14(r29)
    cmpwi r0, 0x0
    bne @L_801B7A78
    li r26, 0x0
    b @L_801B7A54
@L_801B7A18:
    rlwinm r3, r26, 3, 0, 28
    rlwinm r0, r26, 3, 0, 28
    add r3, r29, r3
    add r4, r29, r0
    lwz r3, 0x38(r3)
    lbz r4, 0x35(r4)
    bl fn_801B750C
    rlwinm r3, r26, 3, 0, 28
    rlwinm r0, r26, 3, 0, 28
    add r3, r29, r3
    add r4, r29, r0
    lwz r3, 0x58(r3)
    lbz r4, 0x55(r4)
    bl fn_801B750C
    addi r26, r26, 0x1
@L_801B7A54:
    cmpwi r26, 0x4
    blt @L_801B7A18
    b @L_801B7A78
@L_801B7A60:
    lbz r0, 0x10(r29)
    cmplwi r0, 0x0
    beq @L_801B7A78
    lbz r3, 0x10(r29)
    subi r0, r3, 0x1
    stb r0, 0x10(r29)
@L_801B7A78:
    rlwinm r0, r30, 3, 0, 28
    rlwinm r4, r30, 3, 0, 28
    add r3, r31, r0
    lwz r29, 0x58(r3)
    add r3, r31, r4
    lbz r26, 0x55(r3)
    mr r3, r29
    bl fn_801B7C60
    cmpwi r3, 0x4
    beq @L_801B7B58
    bge @L_801B7B70
    cmpwi r3, 0x1
    beq @L_801B7AB0
    b @L_801B7B70
@L_801B7AB0:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B7AD8
    lwz r0, 0x8(r29)
    cmpwi r0, 0x0
    beq @L_801B7AF0
    lwz r3, 0x8(r29)
    subi r0, r3, 0x1
    stw r0, 0x8(r29)
    b @L_801B7AF0
@L_801B7AD8:
    lwz r0, 0x14(r29)
    cmpwi r0, 0x0
    beq @L_801B7AF0
    lwz r3, 0x14(r29)
    subi r0, r3, 0x1
    stw r0, 0x14(r29)
@L_801B7AF0:
    lwz r0, 0x8(r29)
    cmpwi r0, 0x0
    bne @L_801B7B70
    lwz r0, 0x14(r29)
    cmpwi r0, 0x0
    bne @L_801B7B70
    li r26, 0x0
    b @L_801B7B4C
@L_801B7B10:
    rlwinm r3, r26, 3, 0, 28
    rlwinm r0, r26, 3, 0, 28
    add r3, r29, r3
    add r4, r29, r0
    lwz r3, 0x38(r3)
    lbz r4, 0x35(r4)
    bl fn_801B750C
    rlwinm r3, r26, 3, 0, 28
    rlwinm r0, r26, 3, 0, 28
    add r3, r29, r3
    add r4, r29, r0
    lwz r3, 0x58(r3)
    lbz r4, 0x55(r4)
    bl fn_801B750C
    addi r26, r26, 0x1
@L_801B7B4C:
    cmpwi r26, 0x4
    blt @L_801B7B10
    b @L_801B7B70
@L_801B7B58:
    lbz r0, 0x10(r29)
    cmplwi r0, 0x0
    beq @L_801B7B70
    lbz r3, 0x10(r29)
    subi r0, r3, 0x1
    stb r0, 0x10(r29)
@L_801B7B70:
    addi r30, r30, 0x1
@L_801B7B74:
    cmpwi r30, 0x4
    blt @L_801B7980
    b @L_801B7B98
@L_801B7B80:
    lbz r0, 0x10(r31)
    cmplwi r0, 0x0
    beq @L_801B7B98
    lbz r3, 0x10(r31)
    subi r0, r3, 0x1
    stb r0, 0x10(r31)
@L_801B7B98:
    addi r28, r28, 0x1
@L_801B7B9C:
    cmpwi r28, 0x4
    blt @L_801B75D0
    b @L_801B7BC0
@L_801B7BA8:
    lbz r0, 0x10(r27)
    cmplwi r0, 0x0
    beq @L_801B7BC0
    lbz r3, 0x10(r27)
    subi r0, r3, 0x1
    stb r0, 0x10(r27)
@L_801B7BC0:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
