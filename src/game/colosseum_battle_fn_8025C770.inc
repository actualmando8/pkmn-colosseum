    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x43
    li r6, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    li r4, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r4, r3, 16
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025C7F0
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_8021B364
    clrlwi r3, r3, 24
    li r0, 0x1
    subfc r0, r0, r3
    li r0, -0x1
    subfze r0, r0
    clrlwi r3, r0, 24
    b @L_8025C7F4
@L_8025C7F0:
    li r3, 0x1
@L_8025C7F4:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
