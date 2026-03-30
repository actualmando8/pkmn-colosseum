    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r29, 0x34(r1)
    mr r30, r4
    mr r29, r3
    mr r31, r5
    li r4, 0x0
    mr r3, r30
    bl fn_801F453C
    rlwinm r0, r31, 0, 24, 31
    mr r31, r3
    cmplwi r0, 0x1
    bne @L_80235BCC
    li r0, 0x0
    li r4, 0xd
    lis r3, -32733
    stw r4, 0x1c(r1)
    addi r4, r3, 25192
    addi r5, r1, 0x1c
    stw r0, 0x20(r1)
    mr r3, r30
    li r6, 0x0
    stw r0, 0x24(r1)
    stw r0, 0x28(r1)
    stw r29, 0x2c(r1)
    bl fn_801F37B0
    lwz r0, 0x20(r1)
    cmplwi r0, 0x0
    beq @L_80235B84
    li r3, 0x0
    b @L_80235BD0
@L_80235B84:
    li r0, 0x0
    li r4, 0x4d
    lis r3, -32733
    stw r4, 0x8(r1)
    addi r4, r3, 25192
    addi r5, r1, 0x8
    stw r0, 0xc(r1)
    mr r3, r30
    li r6, 0x0
    stw r0, 0x10(r1)
    stw r0, 0x14(r1)
    stw r29, 0x18(r1)
    bl fn_801F37B0
    lwz r0, 0xc(r1)
    cmplwi r0, 0x0
    beq @L_80235BCC
    li r3, 0x0
    b @L_80235BD0
@L_80235BCC:
    mr r3, r31
@L_80235BD0:
    lmw r29, 0x34(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
