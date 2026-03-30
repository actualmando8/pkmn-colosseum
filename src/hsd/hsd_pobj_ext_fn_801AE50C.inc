    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    b @L_801AE5C0
@L_801AE530:
    cmplwi r28, 0x0
    lwz r29, 0x0(r28)
    beq @L_801AE5BC
    lwz r3, 0x4(r28)
    lis r0, 0x1000
    rlwinm r3, r3, 0, 1, 3
    cmpw r3, r0
    beq @L_801AE560
    bge @L_801AE5A4
    cmpwi r3, 0x0
    beq @L_801AE56C
    b @L_801AE5A4
@L_801AE560:
    lwz r3, 0x8(r28)
    bl fn_801A053C
    b @L_801AE5A4
@L_801AE56C:
    lwz r30, 0xc(r28)
    b @L_801AE59C
@L_801AE574:
    cmplwi r30, 0x0
    lwz r31, 0x0(r30)
    beq @L_801AE598
    lwz r3, 0x8(r30)
    bl fn_801A053C
    lis r3, -32698
    mr r4, r30
    addi r3, r3, 22152
    bl fn_801AA498
@L_801AE598:
    mr r30, r31
@L_801AE59C:
    cmplwi r30, 0x0
    bne @L_801AE574
@L_801AE5A4:
    lwz r3, 0x18(r28)
    bl fn_801C25E4
    lis r3, -32698
    mr r4, r28
    addi r3, r3, 22196
    bl fn_801AA498
@L_801AE5BC:
    mr r28, r29
@L_801AE5C0:
    cmplwi r28, 0x0
    bne @L_801AE530
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
