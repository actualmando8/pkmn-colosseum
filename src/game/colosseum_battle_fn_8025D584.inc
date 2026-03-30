    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_8006B09C
    lwz r4, 0x20(r3)
    subic. r0, r4, 0x1
    blt @L_8025D5A8
    cmpwi r0, 0x6
    ble @L_8025D5B0
@L_8025D5A8:
    li r3, 0x0
    b @L_8025D5D0
@L_8025D5B0:
    slwi r0, r0, 2
    li r5, -0x1
    add r4, r3, r0
    stw r5, 0x8(r4)
    lwz r4, 0x20(r3)
    subi r0, r4, 0x1
    stw r0, 0x20(r3)
    lwz r3, 0x20(r3)
@L_8025D5D0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
