    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047ACE0(r13)
    cmplwi r0, 0x18
    bge @L_800FF51C
    lis r5, lbl_80404918@ha
    rlwinm r6, r0, 4, 0, 27
    addi r0, r5, lbl_80404918@l
    li r5, 0xc
    add r6, r0, r6
    stb r4, 0x0(r6)
    mr r4, r3
    addi r3, r6, 0x4
    bl memcpy
    lwz r3, lbl_8047ACE0(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047ACE0(r13)
@L_800FF51C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

