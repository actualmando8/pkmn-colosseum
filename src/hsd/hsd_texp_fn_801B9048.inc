    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    li r31, 0x0
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x4d(r3)
    cmplwi r0, 0x7
    beq @L_801B90FC
    lwz r3, 0x50(r30)
    bl fn_801B7C60
    cmpwi r3, 0x1
    beq @L_801B9084
    b @L_801B90F8
@L_801B9084:
    lbz r0, 0x4d(r30)
    cmplwi r0, 0x1
    bne @L_801B90C4
    lwz r3, 0x50(r30)
    lbz r0, 0xe(r3)
    cmplwi r0, 0x1
    beq @L_801B90BC
    lwz r3, 0x50(r30)
    lbz r0, 0x11(r3)
    cmplwi r0, 0x1
    bne @L_801B90BC
    li r0, 0x1
    stb r0, 0x11(r30)
    b @L_801B91D4
@L_801B90BC:
    addi r31, r31, 0x100
    b @L_801B90FC
@L_801B90C4:
    lwz r3, 0x50(r30)
    lbz r0, 0x1a(r3)
    cmplwi r0, 0x1
    beq @L_801B90F0
    lwz r3, 0x50(r30)
    lbz r0, 0x1d(r3)
    cmplwi r0, 0x1
    bne @L_801B90F0
    li r0, 0x1
    stb r0, 0x11(r30)
    b @L_801B91D4
@L_801B90F0:
    addi r31, r31, 0x100
    b @L_801B90FC
@L_801B90F8:
    addi r31, r31, 0x100
@L_801B90FC:
    li r4, 0x0
    b @L_801B913C
@L_801B9104:
    rlwinm r0, r4, 3, 0, 28
    add r3, r30, r0
    lbz r0, 0x35(r3)
    cmplwi r0, 0x7
    beq @L_801B9138
    lbz r0, 0xd(r30)
    cmplwi r0, 0x1
    bne @L_801B9130
    li r0, 0x1
    stb r0, 0x11(r30)
    b @L_801B91D4
@L_801B9130:
    addi r31, r31, 0x100
    b @L_801B9144
@L_801B9138:
    addi r4, r4, 0x1
@L_801B913C:
    cmpwi r4, 0x3
    blt @L_801B9104
@L_801B9144:
    lbz r0, 0xf(r30)
    cmpwi r0, 0x2
    beq @L_801B9168
    bge @L_801B9174
    cmpwi r0, 0x1
    bge @L_801B9160
    b @L_801B9174
@L_801B9160:
    addi r31, r31, 0x80
    b @L_801B9174
@L_801B9168:
    li r0, 0x1
    stb r0, 0x11(r30)
    b @L_801B91D4
@L_801B9174:
    lbz r0, 0x10(r30)
    cmpwi r0, 0x2
    beq @L_801B91A4
    bge @L_801B9190
    cmpwi r0, 0x1
    bge @L_801B919C
    b @L_801B91B8
@L_801B9190:
    cmpwi r0, 0x4
    bge @L_801B91B8
    b @L_801B91AC
@L_801B919C:
    rlwinm r31, r31, 1, 0, 30
    b @L_801B91B8
@L_801B91A4:
    rlwinm r31, r31, 2, 0, 29
    b @L_801B91B8
@L_801B91AC:
    rlwinm r0, r31, 1, 31, 31
    add r0, r0, r31
    srawi r31, r0, 1
@L_801B91B8:
    li r0, 0x100
    xor r0, r31, r0
    srawi r3, r0, 1
    and r0, r0, r31
    subf r0, r0, r3
    rlwinm r0, r0, 1, 31, 31
    stb r0, 0x11(r30)
@L_801B91D4:
    lbz r0, 0x6d(r30)
    li r31, 0x0
    cmplwi r0, 0x7
    beq @L_801B9230
    lwz r3, 0x70(r30)
    bl fn_801B7C60
    cmpwi r3, 0x1
    beq @L_801B91F8
    b @L_801B922C
@L_801B91F8:
    lwz r3, 0x70(r30)
    lbz r0, 0x1a(r3)
    cmplwi r0, 0x1
    beq @L_801B9224
    lwz r3, 0x70(r30)
    lbz r0, 0x1d(r3)
    cmplwi r0, 0x1
    bne @L_801B9224
    li r0, 0x1
    stb r0, 0x1d(r30)
    b @L_801B9308
@L_801B9224:
    addi r31, r31, 0x100
    b @L_801B9230
@L_801B922C:
    addi r31, r31, 0x100
@L_801B9230:
    li r4, 0x0
    b @L_801B9270
@L_801B9238:
    rlwinm r0, r4, 3, 0, 28
    add r3, r30, r0
    lbz r0, 0x55(r3)
    cmplwi r0, 0x7
    beq @L_801B926C
    lbz r0, 0x19(r30)
    cmplwi r0, 0x1
    bne @L_801B9264
    li r0, 0x1
    stb r0, 0x1d(r30)
    b @L_801B9308
@L_801B9264:
    addi r31, r31, 0x100
    b @L_801B9278
@L_801B926C:
    addi r4, r4, 0x1
@L_801B9270:
    cmpwi r4, 0x3
    blt @L_801B9238
@L_801B9278:
    lbz r0, 0x1b(r30)
    cmpwi r0, 0x2
    beq @L_801B929C
    bge @L_801B92A8
    cmpwi r0, 0x1
    bge @L_801B9294
    b @L_801B92A8
@L_801B9294:
    addi r31, r31, 0x80
    b @L_801B92A8
@L_801B929C:
    li r0, 0x1
    stb r0, 0x1d(r30)
    b @L_801B9308
@L_801B92A8:
    lbz r0, 0x1c(r30)
    cmpwi r0, 0x2
    beq @L_801B92D8
    bge @L_801B92C4
    cmpwi r0, 0x1
    bge @L_801B92D0
    b @L_801B92EC
@L_801B92C4:
    cmpwi r0, 0x4
    bge @L_801B92EC
    b @L_801B92E0
@L_801B92D0:
    rlwinm r31, r31, 1, 0, 30
    b @L_801B92EC
@L_801B92D8:
    rlwinm r31, r31, 2, 0, 29
    b @L_801B92EC
@L_801B92E0:
    rlwinm r0, r31, 1, 31, 31
    add r0, r0, r31
    srawi r31, r0, 1
@L_801B92EC:
    li r0, 0x100
    xor r0, r31, r0
    srawi r3, r0, 1
    and r0, r0, r31
    subf r0, r0, r3
    rlwinm r0, r0, 1, 31, 31
    stb r0, 0x1d(r30)
@L_801B9308:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
