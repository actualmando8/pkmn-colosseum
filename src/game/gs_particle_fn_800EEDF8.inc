    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r1
    stw r30, 0x8(r1)
    mr r30, r3
    mflr r3
    li r7, 0x0
    stw r3, lbl_8047ABE4(r13)
    stw r7, lbl_8047ABEC(r13)
    lis r4, lbl_804018F0@ha
    li r0, 0x4
    addi r4, r4, lbl_804018F0@l
    li r6, 0xff
    mtctr r0
@L_800EEE38:
    stbx r6, r4, r7
    addi r7, r7, 0x8
    stbx r6, r4, r7
    addi r7, r7, 0x8
    stbx r6, r4, r7
    addi r7, r7, 0x8
    stbx r6, r4, r7
    addi r7, r7, 0x8
    stbx r6, r4, r7
    addi r7, r7, 0x8
    stbx r6, r4, r7
    addi r7, r7, 0x8
    stbx r6, r4, r7
    addi r7, r7, 0x8
    stbx r6, r4, r7
    addi r7, r7, 0x8
    bdnz @L_800EEE38
    bl fn_8009B4D8
    clrlslwi r0, r30, 24, 9
    lis r6, 0xe000
    rlwinm. r4, r30, 0, 24, 31
    stw r0, lbl_8047ABD8(r13)
    stw r6, lbl_8047ABE0(r13)
    beq @L_800EEEE8
    mr r7, r30
    li r6, 0x0
    lis r4, 0x8000
    b @L_800EEEAC
@L_800EEEA8:
    rlwinm r4, r4, 31, 1, 31
@L_800EEEAC:
    rlwinm. r0, r6, 0, 24, 31
    subi r6, r6, 0x1
    bne @L_800EEEA8
    b @L_800EEECC
@L_800EEEBC:
    lwz r0, lbl_8047ABEC(r13)
    or r0, r0, r4
    rlwinm r4, r4, 31, 1, 31
    stw r0, lbl_8047ABEC(r13)
@L_800EEECC:
    rlwinm. r0, r7, 0, 24, 31
    subi r7, r7, 0x1
    bne @L_800EEEBC
    li r0, 0x0
    lis r4, lbl_804018F0@ha
    stbu r0, lbl_804018F0@l(r4)
    stb r30, 0x1(r4)
@L_800EEEE8:
    li r4, 0x0
    rlwinm. r0, r30, 0, 24, 31
    stb r4, lbl_8047ABE8(r13)
    bne @L_800EEF00
    li r3, 0x1
    b @L_800EEF28
@L_800EEF00:
    lwz r3, lbl_8047ABE0(r13)
    lwz r5, lbl_8047ABD8(r13)
    add r3, r3, r5
    subi r1, r3, 0x8
    li r3, -0x1
    stw r3, 0x0(r1)
    lwz r3, lbl_8047ABE4(r13)
    mtlr r3
    blr
    li r3, 0x1
@L_800EEF28:
    mr r10, r31
    lwz r31, 0xc(r31)
    lwz r30, 0x8(r10)
    lwz r10, 0x0(r1)
    lwz r0, 0x4(r10)
    mr r1, r10
    mtlr r0
    blr

