    nofralloc
    lis r3, lbl_80379F58@ha
    li r7, 0x0
    addi r3, r3, lbl_80379F58@l
    addis r3, r3, 0x1
    lbz r3, 0x601e(r3)
    rlwinm r0, r3, 0, 24, 27
    cmpwi r0, 0x90
    beq @L_802210C4
    bge @L_802210A8
    cmpwi r0, 0x20
    beq @L_802210BC
    bge @L_802210D0
    cmpwi r0, 0x10
    beq @L_802210B4
    b @L_802210D0
@L_802210A8:
    cmpwi r0, 0xa0
    beq @L_802210CC
    b @L_802210D0
@L_802210B4:
    li r7, 0xf
    b @L_802210D0
@L_802210BC:
    li r7, 0x27
    b @L_802210D0
@L_802210C4:
    li r7, 0x16
    b @L_802210D0
@L_802210CC:
    li r7, 0x2e
@L_802210D0:
    rlwinm r6, r3, 0, 28, 31
    lwz r3, lbl_8047B610(r13)
    lis r5, lbl_80379F58@ha
    li r4, 0x0
    addi r0, r3, 0x1
    subi r6, r6, 0x1
    addi r3, r5, lbl_80379F58@l
    stw r0, lbl_8047B610(r13)
    addis r3, r3, 0x1
    add r0, r7, r6
    stb r0, 0x60a4(r3)
    stb r4, 0x60a5(r3)
    blr
