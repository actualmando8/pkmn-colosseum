    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    rlwinm r0, r6, 3, 0, 28
    stmw r26, 0x18(r1)
    mr r26, r3
    mr. r28, r5
    lis r3, -32729
    add r5, r26, r0
    mr r29, r6
    addi r31, r3, 21416
    mr r27, r4
    lwz r3, 0x34(r5)
    lwz r0, 0x38(r5)
    stw r3, 0x8(r1)
    stw r0, 0xc(r1)
    bne @L_801B663C
    li r6, 0x0
    b @L_801B6668
@L_801B663C:
    addis r0, r28, 0x1
    cmplwi r0, 0xffff
    bne @L_801B6650
    li r6, 0x2
    b @L_801B6668
@L_801B6650:
    addis r0, r28, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B6664
    li r6, 0x3
    b @L_801B6668
@L_801B6664:
    lwz r6, 0x0(r28)
@L_801B6668:
    rlwinm r0, r29, 3, 0, 28
    rlwinm r4, r29, 3, 0, 28
    add r5, r26, r0
    rlwinm r3, r29, 3, 0, 28
    stb r6, 0x34(r5)
    add r5, r26, r4
    add r4, r26, r3
    rlwinm r0, r29, 3, 0, 28
    stb r27, 0x35(r5)
    add r3, r26, r0
    cmpwi r27, 0xc
    li r0, 0xff
    stw r28, 0x38(r4)
    stb r0, 0x36(r3)
    beq @L_801B6734
    bge @L_801B66C0
    cmpwi r27, 0x8
    beq @L_801B6700
    bge @L_801B6768
    cmpwi r27, 0x7
    bge @L_801B66CC
    b @L_801B6824
@L_801B66C0:
    cmpwi r27, 0x10
    bge @L_801B6824
    b @L_801B6768
@L_801B66CC:
    rlwinm r0, r29, 3, 0, 28
    rlwinm r3, r29, 3, 0, 28
    add r4, r26, r0
    li r0, 0xf
    stb r0, 0x36(r4)
    rlwinm r0, r29, 3, 0, 28
    add r3, r26, r3
    li r4, 0x0
    stb r4, 0x34(r3)
    add r3, r26, r0
    li r0, 0x0
    stw r0, 0x38(r3)
    b @L_801B6CB8
@L_801B6700:
    rlwinm r0, r29, 3, 0, 28
    rlwinm r3, r29, 3, 0, 28
    add r4, r26, r0
    li r0, 0xc
    stb r0, 0x36(r4)
    rlwinm r0, r29, 3, 0, 28
    add r3, r26, r3
    li r4, 0x5
    stb r4, 0x34(r3)
    add r3, r26, r0
    li r0, 0x0
    stw r0, 0x38(r3)
    b @L_801B6CB8
@L_801B6734:
    rlwinm r0, r29, 3, 0, 28
    rlwinm r3, r29, 3, 0, 28
    add r4, r26, r0
    li r0, 0xd
    stb r0, 0x36(r4)
    rlwinm r0, r29, 3, 0, 28
    add r3, r26, r3
    li r4, 0x5
    stb r4, 0x34(r3)
    add r3, r26, r0
    li r0, 0x0
    stw r0, 0x38(r3)
    b @L_801B6CB8
@L_801B6768:
    rlwinm r3, r29, 3, 0, 28
    subi r0, r27, 0x9
    cmplwi r0, 0x6
    li r4, 0xe
    add r3, r26, r3
    stb r4, 0x36(r3)
    bgt @L_801B67CC
    lis r3, -32713
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -11424
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r30, 0x7
    b @L_801B67DC
    li r30, 0x6
    b @L_801B67DC
    li r30, 0x5
    b @L_801B67DC
    li r30, 0x3
    b @L_801B67DC
    li r30, 0x2
    b @L_801B67DC
    li r30, 0x1
    b @L_801B67DC
@L_801B67CC:
    addi r3, r2, -22576
    li r4, 0x213
    addi r5, r2, -22528
    bl fn_80196E10
@L_801B67DC:
    lbz r0, 0x20(r26)
    cmplwi r0, 0xff
    bne @L_801B67F0
    stb r30, 0x20(r26)
    b @L_801B6810
@L_801B67F0:
    lbz r3, 0x20(r26)
    rlwinm r0, r30, 0, 24, 31
    cmplw r3, r0
    bne @L_801B6810
    addi r5, r31, 0x114
    addi r3, r2, -22576
    li r4, 0x218
    bl fn_80196D78
@L_801B6810:
    rlwinm r0, r29, 3, 0, 28
    li r4, 0x6
    add r3, r26, r0
    stb r4, 0x34(r3)
    b @L_801B6CB8
@L_801B6824:
    rlwinm r0, r29, 3, 0, 28
    add r3, r26, r0
    lbz r0, 0x34(r3)
    cmpwi r0, 0x2
    beq @L_801B6A98
    bge @L_801B684C
    cmpwi r0, 0x0
    beq @L_801B685C
    bge @L_801B6890
    b @L_801B6CA8
@L_801B684C:
    cmpwi r0, 0x4
    beq @L_801B69E0
    bge @L_801B6CA8
    b @L_801B6BA0
@L_801B685C:
    rlwinm r0, r29, 3, 0, 28
    rlwinm r3, r29, 3, 0, 28
    add r4, r26, r0
    li r0, 0x0
    stb r0, 0x34(r4)
    rlwinm r0, r29, 3, 0, 28
    add r3, r26, r3
    li r4, 0x7
    stb r4, 0x35(r3)
    add r3, r26, r0
    li r0, 0xf
    stb r0, 0x36(r3)
    b @L_801B6CB8
@L_801B6890:
    cmpwi r27, 0x1
    li r0, 0x0
    beq @L_801B68A4
    cmpwi r27, 0x5
    bne @L_801B68A8
@L_801B68A4:
    li r0, 0x1
@L_801B68A8:
    cmpwi r0, 0x0
    bne @L_801B68C0
    addi r5, r31, 0x13c
    addi r3, r2, -22576
    li r4, 0x228
    bl fn_80196E10
@L_801B68C0:
    cmpwi r29, 0x3
    li r3, 0x0
    beq @L_801B68E0
    cmpwi r27, 0x1
    bne @L_801B68E0
    lbz r0, 0xe(r28)
    cmplwi r0, 0x0
    beq @L_801B68E4
@L_801B68E0:
    li r3, 0x1
@L_801B68E4:
    cmpwi r3, 0x0
    bne @L_801B68FC
    addi r5, r31, 0x164
    addi r3, r2, -22576
    li r4, 0x22a
    bl fn_80196E10
@L_801B68FC:
    cmpwi r29, 0x3
    li r3, 0x0
    beq @L_801B691C
    cmpwi r27, 0x5
    bne @L_801B691C
    lbz r0, 0x1a(r28)
    cmplwi r0, 0x0
    beq @L_801B6920
@L_801B691C:
    li r3, 0x1
@L_801B6920:
    cmpwi r3, 0x0
    bne @L_801B6938
    addi r5, r31, 0x198
    addi r3, r2, -22576
    li r4, 0x22b
    bl fn_80196E10
@L_801B6938:
    rlwinm r0, r29, 3, 0, 28
    rlwinm r4, r29, 3, 0, 28
    add r3, r26, r0
    lwz r5, 0x38(r3)
    add r3, r26, r4
    lbz r3, 0x35(r3)
    cmplwi r5, 0x0
    bne @L_801B6960
    li r0, 0x0
    b @L_801B698C
@L_801B6960:
    addis r0, r5, 0x1
    cmplwi r0, 0xffff
    bne @L_801B6974
    li r0, 0x2
    b @L_801B698C
@L_801B6974:
    addis r0, r5, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B6988
    li r0, 0x3
    b @L_801B698C
@L_801B6988:
    lwz r0, 0x0(r5)
@L_801B698C:
    cmpwi r0, 0x4
    beq @L_801B69D0
    bge @L_801B6CB8
    cmpwi r0, 0x1
    beq @L_801B69A4
    b @L_801B6CB8
@L_801B69A4:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B69C0
    lwz r3, 0x8(r5)
    addi r0, r3, 0x1
    stw r0, 0x8(r5)
    b @L_801B6CB8
@L_801B69C0:
    lwz r3, 0x14(r5)
    addi r0, r3, 0x1
    stw r0, 0x14(r5)
    b @L_801B6CB8
@L_801B69D0:
    lbz r3, 0x10(r5)
    addi r0, r3, 0x1
    stb r0, 0x10(r5)
    b @L_801B6CB8
@L_801B69E0:
    rlwinm r0, r29, 3, 0, 28
    lbz r4, 0xc(r28)
    add r3, r26, r0
    rlwinm r0, r29, 3, 0, 28
    stb r4, 0x35(r3)
    add r3, r26, r0
    rlwinm r0, r29, 3, 0, 28
    lwz r4, 0x38(r3)
    add r3, r26, r0
    lbz r3, 0x35(r3)
    cmplwi r4, 0x0
    bne @L_801B6A18
    li r0, 0x0
    b @L_801B6A44
@L_801B6A18:
    addis r0, r4, 0x1
    cmplwi r0, 0xffff
    bne @L_801B6A2C
    li r0, 0x2
    b @L_801B6A44
@L_801B6A2C:
    addis r0, r4, 0x1
    cmplwi r0, 0xfffe
    bne @L_801B6A40
    li r0, 0x3
    b @L_801B6A44
@L_801B6A40:
    lwz r0, 0x0(r4)
@L_801B6A44:
    cmpwi r0, 0x4
    beq @L_801B6A88
    bge @L_801B6CB8
    cmpwi r0, 0x1
    beq @L_801B6A5C
    b @L_801B6CB8
@L_801B6A5C:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_801B6A78
    lwz r3, 0x8(r4)
    addi r0, r3, 0x1
    stw r0, 0x8(r4)
    b @L_801B6CB8
@L_801B6A78:
    lwz r3, 0x14(r4)
    addi r0, r3, 0x1
    stw r0, 0x14(r4)
    b @L_801B6CB8
@L_801B6A88:
    lbz r3, 0x10(r4)
    addi r0, r3, 0x1
    stb r0, 0x10(r4)
    b @L_801B6CB8
@L_801B6A98:
    cmpwi r27, 0x3
    li r30, 0xff
    beq @L_801B6AF8
    bge @L_801B6AB8
    cmpwi r27, 0x1
    beq @L_801B6AC8
    bge @L_801B6AE0
    b @L_801B6B3C
@L_801B6AB8:
    cmpwi r27, 0x5
    beq @L_801B6B28
    bge @L_801B6B3C
    b @L_801B6B10
@L_801B6AC8:
    rlwinm r0, r29, 3, 0, 28
    li r4, 0x8
    add r3, r26, r0
    li r30, 0x0
    stb r4, 0x36(r3)
    b @L_801B6B4C
@L_801B6AE0:
    rlwinm r0, r29, 3, 0, 28
    li r4, 0x8
    add r3, r26, r0
    li r30, 0x1
    stb r4, 0x36(r3)
    b @L_801B6B4C
@L_801B6AF8:
    rlwinm r0, r29, 3, 0, 28
    li r4, 0x8
    add r3, r26, r0
    li r30, 0x2
    stb r4, 0x36(r3)
    b @L_801B6B4C
@L_801B6B10:
    rlwinm r0, r29, 3, 0, 28
    li r4, 0x8
    add r3, r26, r0
    li r30, 0x3
    stb r4, 0x36(r3)
    b @L_801B6B4C
@L_801B6B28:
    rlwinm r0, r29, 3, 0, 28
    li r4, 0x9
    add r3, r26, r0
    stb r4, 0x36(r3)
    b @L_801B6B4C
@L_801B6B3C:
    addi r3, r2, -22576
    li r4, 0x24e
    addi r5, r2, -22528
    bl fn_80196E10
@L_801B6B4C:
    lbz r0, 0x1e(r26)
    cmplwi r0, 0xff
    bne @L_801B6B60
    stb r30, 0x1e(r26)
    b @L_801B6CB8
@L_801B6B60:
    rlwinm r0, r30, 0, 24, 31
    li r4, 0x0
    cmplwi r0, 0xff
    beq @L_801B6B80
    lbz r3, 0x1e(r26)
    rlwinm r0, r30, 0, 24, 31
    cmplw r3, r0
    bne @L_801B6B84
@L_801B6B80:
    li r4, 0x1
@L_801B6B84:
    cmpwi r4, 0x0
    bne @L_801B6CB8
    addi r5, r31, 0x1c8
    addi r3, r2, -22576
    li r4, 0x253
    bl fn_80196E10
    b @L_801B6CB8
@L_801B6BA0:
    cmpwi r27, 0x3
    li r30, 0xff
    beq @L_801B6C00
    bge @L_801B6BC0
    cmpwi r27, 0x1
    beq @L_801B6BD0
    bge @L_801B6BE8
    b @L_801B6C44
@L_801B6BC0:
    cmpwi r27, 0x5
    beq @L_801B6C30
    bge @L_801B6C44
    b @L_801B6C18
@L_801B6BD0:
    rlwinm r0, r29, 3, 0, 28
    li r4, 0xa
    add r3, r26, r0
    li r30, 0x0
    stb r4, 0x36(r3)
    b @L_801B6C54
@L_801B6BE8:
    rlwinm r0, r29, 3, 0, 28
    li r4, 0xa
    add r3, r26, r0
    li r30, 0x1
    stb r4, 0x36(r3)
    b @L_801B6C54
@L_801B6C00:
    rlwinm r0, r29, 3, 0, 28
    li r4, 0xa
    add r3, r26, r0
    li r30, 0x2
    stb r4, 0x36(r3)
    b @L_801B6C54
@L_801B6C18:
    rlwinm r0, r29, 3, 0, 28
    li r4, 0xa
    add r3, r26, r0
    li r30, 0x3
    stb r4, 0x36(r3)
    b @L_801B6C54
@L_801B6C30:
    rlwinm r0, r29, 3, 0, 28
    li r4, 0xb
    add r3, r26, r0
    stb r4, 0x36(r3)
    b @L_801B6C54
@L_801B6C44:
    addi r3, r2, -22576
    li r4, 0x27e
    addi r5, r2, -22528
    bl fn_80196E10
@L_801B6C54:
    lbz r0, 0x1f(r26)
    cmplwi r0, 0xff
    bne @L_801B6C68
    stb r30, 0x1f(r26)
    b @L_801B6CB8
@L_801B6C68:
    rlwinm r0, r30, 0, 24, 31
    li r4, 0x0
    cmplwi r0, 0xff
    beq @L_801B6C88
    lbz r3, 0x1f(r26)
    rlwinm r0, r30, 0, 24, 31
    cmplw r3, r0
    bne @L_801B6C8C
@L_801B6C88:
    li r4, 0x1
@L_801B6C8C:
    cmpwi r4, 0x0
    bne @L_801B6CB8
    addi r5, r31, 0x1f8
    addi r3, r2, -22576
    li r4, 0x283
    bl fn_80196E10
    b @L_801B6CB8
@L_801B6CA8:
    addi r3, r2, -22576
    li r4, 0x295
    addi r5, r2, -22528
    bl fn_80196E10
@L_801B6CB8:
    lwz r3, 0xc(r1)
    lbz r4, 0x9(r1)
    bl fn_801B750C
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
