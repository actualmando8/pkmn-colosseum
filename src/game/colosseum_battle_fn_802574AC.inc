    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    mr r3, r5
    mr r5, r6
    bl fn_80229934
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802574E0
    li r3, 0x0
    b @L_802574F4
@L_802574E0:
    mr r3, r31
    bl fn_8025C674
    clrlwi r3, r3, 16
    subic r0, r3, 0x1
    subfe r3, r0, r3
@L_802574F4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
