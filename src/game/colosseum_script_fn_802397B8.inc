    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r9, -0xc8
    li r8, 0x0
    stw r0, 0x54(r1)
    addi r6, r1, 0x8
    li r7, -0x1
    stw r31, 0x4c(r1)
    li r31, 0x0
    b @L_802397EC
@L_802397E0:
    clrlslwi r0, r8, 16, 2
    addi r8, r8, 0x1
    stwx r7, r6, r0
@L_802397EC:
    rlwinm r0, r8, 0, 16, 31
    cmplwi r0, 0x10
    blt @L_802397E0
    rlwinm r0, r4, 0, 16, 31
    li r7, 0x0
    b @L_8023981C
@L_80239804:
    clrlslwi r6, r7, 16, 2
    lwzx r6, r3, r6
    cmpw r9, r6
    bge @L_80239818
    mr r9, r6
@L_80239818:
    addi r7, r7, 0x1
@L_8023981C:
    rlwinm r6, r7, 0, 16, 31
    cmplw r6, r0
    blt @L_80239804
    addi r6, r1, 0x8
    rlwinm r0, r4, 0, 16, 31
    li r8, 0x0
    b @L_80239868
@L_80239838:
    clrlslwi r4, r8, 16, 2
    rlwinm r7, r8, 0, 16, 31
    lwzx r4, r3, r4
    cmpw r9, r4
    bgt @L_80239858
    clrlslwi r4, r31, 16, 2
    addi r31, r31, 0x1
    stwx r7, r6, r4
@L_80239858:
    rlwinm r4, r31, 0, 16, 31
    cmplwi r4, 0x10
    bge @L_80239874
    addi r8, r8, 0x1
@L_80239868:
    rlwinm r4, r8, 0, 16, 31
    cmplw r4, r0
    blt @L_80239838
@L_80239874:
    rlwinm. r0, r31, 0, 16, 31
    bne @L_80239884
    li r3, -0x1
    b @L_802398D0
@L_80239884:
    cmplwi r0, 0x1
    bne @L_80239894
    li r0, 0x0
    b @L_802398C4
@L_80239894:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802398C0
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    rlwinm r3, r31, 0, 16, 31
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    rlwinm r0, r0, 0, 16, 31
    b @L_802398C4
@L_802398C0:
    li r0, 0x0
@L_802398C4:
    clrlslwi r0, r0, 16, 2
    addi r3, r1, 0x8
    lwzx r3, r3, r0
@L_802398D0:
    lwz r0, 0x54(r1)
    lwz r31, 0x4c(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
