    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_802067A0
    li r3, 0x0
    b @L_802068B4
@L_802067A0:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_802067B8
    li r3, 0x0
    b @L_802068B4
@L_802067B8:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r31, r3
    bne @L_802067DC
    li r3, 0x0
    b @L_802068B4
@L_802067DC:
    bne @L_802067E8
    li r0, 0x0
    b @L_802068A8
@L_802067E8:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_80206800
    li r0, 0x0
    b @L_802068A8
@L_80206800:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcb
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80206824
    li r0, 0x0
    b @L_802068A8
@L_80206824:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80206838
    li r0, 0x0
    b @L_802068A8
@L_80206838:
    cmplwi r31, 0x0
    bne @L_80206848
    li r3, 0x0
    b @L_8020685C
@L_80206848:
    mr r3, r31
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_8020685C:
    cmplwi r3, 0x0
    bne @L_8020686C
    li r0, 0x0
    b @L_802068A8
@L_8020686C:
    bl fn_80123FBC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80206880
    li r0, 0x0
    b @L_802068A8
@L_80206880:
    mr r3, r31
    li r4, 0x0
    li r5, 0xce
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bge @L_802068A4
    li r0, 0x0
    b @L_802068A8
@L_802068A4:
    li r0, 0x1
@L_802068A8:
    rlwinm r3, r0, 0, 24, 31
    subic r0, r3, 0x1
    subfe r3, r0, r3
@L_802068B4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
