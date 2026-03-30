    nofralloc
    clrrwi. r0, r3, 16
    beq @L_800B2FD4
    addis r0, r3, 0x8000
    cmplwi r0, 0x4
    bne @L_800B2FCC
    lhz r0, lbl_80478A58(r13)
    cmplwi r0, 0xffff
    bne @L_800B2FD4
    @L_800B2FCC:
    li r3, 0x0
    blr
    @L_800B2FD4:
    clrlwi. r0, r3, 30
    beq @L_800B2FE4
    li r3, 0x0
    blr
    @L_800B2FE4:
    rlwinm r5, r3, 0, 24, 29
    cmpwi r5, 0x20
    beq @L_800B3034
    bge @L_800B3018
    cmpwi r5, 0x8
    beq @L_800B3034
    bge @L_800B300C
    cmpwi r5, 0x4
    beq @L_800B3034
    b @L_800B302C
    @L_800B300C:
    cmpwi r5, 0x10
    beq @L_800B3034
    b @L_800B302C
    @L_800B3018:
    cmpwi r5, 0x80
    beq @L_800B3034
    bge @L_800B302C
    cmpwi r5, 0x40
    beq @L_800B3034
    @L_800B302C:
    li r3, 0x0
    blr
    @L_800B3034:
    lis r4, lbl_80312960@ha
    rlwinm r3, r3, 23, 27, 29
    addi r0, r4, lbl_80312960@l
    add r3, r0, r3
    lwz r3, 0x0(r3)
    cmpwi r3, 0x0
    bne @L_800B3058
    li r3, 0x0
    blr
    @L_800B3058:
    clrlslwi r0, r5, 20, 17
    divwu r0, r0, r3
    cmplwi r0, 0x8
    bge @L_800B3070
    li r3, 0x0
    blr
    @L_800B3070:
    li r3, 0x1
    blr
