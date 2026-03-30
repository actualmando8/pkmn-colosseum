    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0xd6
    stw r0, 0x14(r1)
    li r6, 0x0
    stmw r30, 0x8(r1)
    bl fn_8012640C
    mr. r30, r3
    bne @L_80203F0C
    li r31, 0x0
    b @L_80203F20
@L_80203F0C:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r31, r3
@L_80203F20:
    cmplwi r31, 0x0
    bne @L_80203F30
    li r3, 0x0
    b @L_80203FD0
@L_80203F30:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80203F58
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_80203F88
@L_80203F58:
    cmplwi r30, 0x0
    bne @L_80203F68
    li r3, 0x0
    b @L_80203F7C
@L_80203F68:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80203F7C:
    li r4, 0x3d
    bl fn_80121ADC
    b @L_80203FB0
@L_80203F88:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_80203FA4
    li r3, 0x0
    b @L_80203FB0
@L_80203FA4:
    mr r3, r30
    li r4, 0x3d
    bl fn_8011B67C
@L_80203FB0:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80203FC4
    li r3, 0x0
    b @L_80203FD0
@L_80203FC4:
    mr r3, r31
    bl fn_80122FF4
    rlwinm r3, r3, 0, 16, 31
@L_80203FD0:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
