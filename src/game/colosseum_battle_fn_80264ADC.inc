    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r28, 0x30(r1)
    mr r30, r3
    mr r31, r5
@L_80264AF4:
    mr r4, r30
    mr r5, r31
    li r3, 0xf
    bl fn_801F02AC
    mr r28, r3
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80264B98
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80264B38
    li r3, 0x0
    b @L_80264B9C
@L_80264B38:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    rlwinm r29, r3, 0, 16, 31
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80264B68
    li r3, 0x0
    b @L_80264B9C
@L_80264B68:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80264B80
    li r3, 0x0
    b @L_80264B9C
@L_80264B80:
    mr r4, r29
    rlwinm r6, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
    b @L_80264B9C
@L_80264B98:
    li r3, 0x0
@L_80264B9C:
    stw r3, 0xc(r1)
    mr r4, r30
    mr r5, r31
    li r3, 0x10
    bl fn_801F02AC
    mr r28, r3
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80264C44
    mr r4, r28
    mr r5, r31
    li r3, 0x2
    bl fn_801F02AC
    cmplwi r3, 0x0
    bne @L_80264BE4
    li r3, 0x0
    b @L_80264C48
@L_80264BE4:
    li r4, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_801F76B8
    rlwinm r29, r3, 0, 16, 31
    mr r3, r28
    mr r4, r31
    bl fn_801F0134
    rlwinm. r0, r3, 0, 16, 31
    bne @L_80264C14
    li r3, 0x0
    b @L_80264C48
@L_80264C14:
    bl fn_801F0234
    bl fn_801F0204
    cmpwi r3, 0x0
    bge @L_80264C2C
    li r3, 0x0
    b @L_80264C48
@L_80264C2C:
    mr r4, r29
    rlwinm r6, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x3
    bl fn_801F76B8
    b @L_80264C48
@L_80264C44:
    li r3, 0x0
@L_80264C48:
    li r4, 0x0
    li r0, 0x2
    stw r3, 0x14(r1)
    li r3, 0x0
    stw r4, 0x1c(r1)
    stw r4, 0x24(r1)
    stb r0, 0x28(r1)
    bl fn_801F18DC
    stb r3, 0x29(r1)
    addi r3, r1, 0x8
    li r4, 0x0
    li r5, 0x1
    bl fn_80011288
    cmpwi r3, 0x0
    bge @L_80264C94
    li r3, 0x1
    bl fn_8001120C
    li r3, 0x0
    b @L_80264D44
@L_80264C94:
    bne @L_80264CB0
    mr r4, r30
    mr r5, r31
    li r3, 0xf
    bl fn_801F02AC
    mr r29, r3
    b @L_80264CCC
@L_80264CB0:
    cmpwi r3, 0x1
    bne @L_80264AF4
    mr r4, r30
    mr r5, r31
    li r3, 0x10
    bl fn_801F02AC
    mr r29, r3
@L_80264CCC:
    mr r3, r29
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80264AF4
    mr r3, r29
    bl fn_80205B8C
    bl fn_8011FC74
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80264D38
    li r3, 0x99f
    bl fn_801906A0
    cmplwi r3, 0x0
    bne @L_80264D14
    li r3, 0x7716
    li r4, 0x1
    li r5, 0x1
    bl fn_80106394
    b @L_80264D24
@L_80264D14:
    li r3, 0x7702
    li r4, 0x1
    li r5, 0x1
    bl fn_80106394
@L_80264D24:
    li r3, 0x40
    bl fn_801F000C
    li r3, 0x0
    bl fn_80106080
    b @L_80264AF4
@L_80264D38:
    li r3, 0x1
    bl fn_8001120C
    mr r3, r29
@L_80264D44:
    lmw r28, 0x30(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
