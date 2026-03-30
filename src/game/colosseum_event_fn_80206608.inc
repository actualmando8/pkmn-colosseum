    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_80206628
    li r3, 0x0
    b @L_8020676C
@L_80206628:
    bne @L_80206634
    li r0, 0x0
    b @L_802066F4
@L_80206634:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_8020664C
    li r0, 0x0
    b @L_802066F4
@L_8020664C:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80206670
    li r0, 0x0
    b @L_802066F4
@L_80206670:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80206684
    li r0, 0x0
    b @L_802066F4
@L_80206684:
    cmplwi r31, 0x0
    bne @L_80206694
    li r3, 0x0
    b @L_802066A8
@L_80206694:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802066A8:
    cmplwi r3, 0x0
    bne @L_802066B8
    li r0, 0x0
    b @L_802066F4
@L_802066B8:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802066CC
    li r0, 0x0
    b @L_802066F4
@L_802066CC:
    mr r3, r31
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_802066F0
    li r0, 0x0
    b @L_802066F4
@L_802066F0:
    li r0, 0x1
@L_802066F4:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80206704
    li r3, 0x0
    b @L_8020676C
@L_80206704:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd2
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    bne @L_80206728
    li r3, 0x0
    b @L_8020676C
@L_80206728:
    cmplwi r31, 0x0
    bne @L_80206738
    li r3, 0x0
    b @L_8020674C
@L_80206738:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_8020674C:
    cmplwi r3, 0x0
    bne @L_8020675C
    li r3, 0x0
    b @L_8020676C
@L_8020675C:
    bl fn_801233F4
    rlwinm r3, r3, 0, 24, 31
    subic r0, r3, 0x1
    subfe r3, r0, r3
@L_8020676C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
