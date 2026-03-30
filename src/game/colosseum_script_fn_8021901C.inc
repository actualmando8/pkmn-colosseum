    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    mr r4, r3
    li r3, 0x0
    stw r0, 0x14(r1)
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x91
    beq @L_80219070
    cmplwi r0, 0x27
    beq @L_80219070
    cmplwi r0, 0x4b
    beq @L_80219070
    cmplwi r0, 0x97
    beq @L_80219070
    cmplwi r0, 0x9b
    beq @L_80219070
    cmplwi r0, 0x1a
    bne @L_80219078
@L_80219070:
    li r3, 0x1
    b @L_8021907C
@L_80219078:
    li r3, 0x0
@L_8021907C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
