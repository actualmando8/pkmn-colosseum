    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
@L_800FF980:
    lwz r0, -23368(r13)
    cmpwi r0, 0x3
    beq @L_800FFACC
    bge @L_800FF9A8
    cmpwi r0, 0x1
    beq @L_800FF9C0
    bge @L_800FFAA4
    cmpwi r0, 0x0
    bge @L_800FF9B8
    b @L_800FF980
@L_800FF9A8:
    cmpwi r0, 0x5
    beq @L_80100480
    bge @L_800FF980
    b @L_800FFFF8
@L_800FF9B8:
    bl fn_800F0308
    b @L_800FF980
@L_800FF9C0:
    lwz r3, -23384(r13)
    lbz r0, 0xa(r3)
    cmplwi r0, 0x0
    beq @L_800FF9D8
    li r25, 0x0
    b @L_800FF9F0
@L_800FF9D8:
    lbz r0, 0xb(r3)
    cmplwi r0, 0x0
    beq @L_800FF9EC
    li r25, 0x1
    b @L_800FF9F0
@L_800FF9EC:
    li r25, 0x2
@L_800FF9F0:
    lwz r29, 0x0(r3)
    mr r4, r25
    mr r3, r29
    bl fn_801123D4
    cmpwi r25, 0x0
    bne @L_800FFA0C
    bl fn_8010D064
@L_800FFA0C:
    cmpwi r25, 0x2
    bne @L_800FFA18
    bl fn_8010CC54
@L_800FFA18:
    mr r3, r29
    bl fn_80115A38
    bl fn_8017B3E4
    lis r3, -32729
    addi r30, r3, 6164
@L_800FFA2C:
    mr r3, r29
    bl fn_80115A38
    bl fn_8017B2CC
    mr. r31, r3
    bge @L_800FFA4C
    mr r3, r30
    crxor 6, 6, 6
    bl fn_800DD970
@L_800FFA4C:
    cmpwi r31, 0x0
    beq @L_800FFA5C
    bl fn_800F0308
    b @L_800FFA2C
@L_800FFA5C:
    mr r3, r29
    bl fn_80115A80
    cmpwi r25, 0x1
    beq @L_800FFA70
    bl fn_8010CD6C
@L_800FFA70:
    bl fn_800F0308
    bl fn_800F0308
    li r0, 0x2
    lwz r3, -23384(r13)
    stw r0, -23368(r13)
    li r4, 0x0
    li r0, 0x1
    stb r4, 0xa(r3)
    lwz r3, -23384(r13)
    stb r4, 0xb(r3)
    lwz r3, -23384(r13)
    stw r0, 0x10(r3)
    b @L_800FF980
@L_800FFAA4:
    lwz r3, -23384(r13)
    bl fn_80100B24
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_800FFAC0
    bl fn_800F0308
    b @L_800FF980
@L_800FFAC0:
    lwz r0, -23364(r13)
    stw r0, -23368(r13)
    b @L_800FF980
@L_800FFACC:
    lwz r3, -23384(r13)
    lwz r29, -23408(r13)
    lwz r31, 0x4(r3)
    lwz r30, -23404(r13)
    b @L_800FFB68
@L_800FFAE0:
    lwz r3, 0xc(r29)
    cmpwi r3, 0x0
    bne @L_800FFB64
    lwz r0, 0x10(r29)
    cmplw r0, r31
    bne @L_800FFB64
    cmpwi r3, 0x1
    bne @L_800FFB14
    lwz r0, 0x1c(r29)
    cmplwi r0, 0x0
    beq @L_800FFB14
    lhz r3, 0x20(r29)
    bl fn_800F7274
@L_800FFB14:
    li r0, 0x0
    stw r0, 0x8(r29)
    lwz r3, 0x0(r29)
    cmplwi r3, 0x0
    beq @L_800FFB30
    lwz r0, 0x4(r29)
    stw r0, 0x4(r3)
@L_800FFB30:
    lwz r3, 0x4(r29)
    cmplwi r3, 0x0
    beq @L_800FFB44
    lwz r0, 0x0(r29)
    stw r0, 0x0(r3)
@L_800FFB44:
    lwz r0, -23380(r13)
    cmplw r0, r29
    bne @L_800FFB58
    lwz r0, 0x4(r29)
    stw r0, -23380(r13)
@L_800FFB58:
    li r0, 0x0
    stw r0, 0x0(r29)
    stw r0, 0x4(r29)
@L_800FFB64:
    addi r29, r29, 0x24
@L_800FFB68:
    cmplwi r30, 0x0
    subi r30, r30, 0x1
    bne @L_800FFAE0
    lwz r0, -23404(r13)
    lwz r4, -23384(r13)
    mulli r0, r0, 0x24
    lwz r3, -23408(r13)
    lwz r31, 0x4(r4)
    lwz r30, -23400(r13)
    add r29, r3, r0
    b @L_800FFC1C
@L_800FFB94:
    lwz r3, 0xc(r29)
    cmpwi r3, 0x0
    bne @L_800FFC18
    lwz r0, 0x10(r29)
    cmplw r0, r31
    bne @L_800FFC18
    cmpwi r3, 0x1
    bne @L_800FFBC8
    lwz r0, 0x1c(r29)
    cmplwi r0, 0x0
    beq @L_800FFBC8
    lhz r3, 0x20(r29)
    bl fn_800F7274
@L_800FFBC8:
    li r0, 0x0
    stw r0, 0x8(r29)
    lwz r3, 0x0(r29)
    cmplwi r3, 0x0
    beq @L_800FFBE4
    lwz r0, 0x4(r29)
    stw r0, 0x4(r3)
@L_800FFBE4:
    lwz r3, 0x4(r29)
    cmplwi r3, 0x0
    beq @L_800FFBF8
    lwz r0, 0x0(r29)
    stw r0, 0x0(r3)
@L_800FFBF8:
    lwz r0, -23380(r13)
    cmplw r0, r29
    bne @L_800FFC0C
    lwz r0, 0x4(r29)
    stw r0, -23380(r13)
@L_800FFC0C:
    li r0, 0x0
    stw r0, 0x0(r29)
    stw r0, 0x4(r29)
@L_800FFC18:
    addi r29, r29, 0x24
@L_800FFC1C:
    cmplwi r30, 0x0
    subi r30, r30, 0x1
    bne @L_800FFB94
    lwz r3, -23404(r13)
    lwz r0, -23400(r13)
    lwz r4, -23384(r13)
    add r0, r3, r0
    lwz r3, -23408(r13)
    mulli r0, r0, 0x24
    lwz r31, 0x4(r4)
    lwz r30, -23396(r13)
    add r29, r3, r0
    b @L_800FFCD8
@L_800FFC50:
    lwz r3, 0xc(r29)
    cmpwi r3, 0x0
    bne @L_800FFCD4
    lwz r0, 0x10(r29)
    cmplw r0, r31
    bne @L_800FFCD4
    cmpwi r3, 0x1
    bne @L_800FFC84
    lwz r0, 0x1c(r29)
    cmplwi r0, 0x0
    beq @L_800FFC84
    lhz r3, 0x20(r29)
    bl fn_800F7274
@L_800FFC84:
    li r0, 0x0
    stw r0, 0x8(r29)
    lwz r3, 0x0(r29)
    cmplwi r3, 0x0
    beq @L_800FFCA0
    lwz r0, 0x4(r29)
    stw r0, 0x4(r3)
@L_800FFCA0:
    lwz r3, 0x4(r29)
    cmplwi r3, 0x0
    beq @L_800FFCB4
    lwz r0, 0x0(r29)
    stw r0, 0x0(r3)
@L_800FFCB4:
    lwz r0, -23380(r13)
    cmplw r0, r29
    bne @L_800FFCC8
    lwz r0, 0x4(r29)
    stw r0, -23380(r13)
@L_800FFCC8:
    li r0, 0x0
    stw r0, 0x0(r29)
    stw r0, 0x4(r29)
@L_800FFCD4:
    addi r29, r29, 0x24
@L_800FFCD8:
    cmplwi r30, 0x0
    subi r30, r30, 0x1
    bne @L_800FFC50
    lwz r3, -23384(r13)
    lwz r29, -23408(r13)
    lwz r31, 0x4(r3)
    lwz r30, -23404(r13)
    b @L_800FFD80
@L_800FFCF8:
    lwz r3, 0xc(r29)
    cmpwi r3, 0x1
    bne @L_800FFD7C
    lwz r0, 0x10(r29)
    cmplw r0, r31
    bne @L_800FFD7C
    cmpwi r3, 0x1
    bne @L_800FFD2C
    lwz r0, 0x1c(r29)
    cmplwi r0, 0x0
    beq @L_800FFD2C
    lhz r3, 0x20(r29)
    bl fn_800F7274
@L_800FFD2C:
    li r0, 0x0
    stw r0, 0x8(r29)
    lwz r3, 0x0(r29)
    cmplwi r3, 0x0
    beq @L_800FFD48
    lwz r0, 0x4(r29)
    stw r0, 0x4(r3)
@L_800FFD48:
    lwz r3, 0x4(r29)
    cmplwi r3, 0x0
    beq @L_800FFD5C
    lwz r0, 0x0(r29)
    stw r0, 0x0(r3)
@L_800FFD5C:
    lwz r0, -23380(r13)
    cmplw r0, r29
    bne @L_800FFD70
    lwz r0, 0x4(r29)
    stw r0, -23380(r13)
@L_800FFD70:
    li r0, 0x0
    stw r0, 0x0(r29)
    stw r0, 0x4(r29)
@L_800FFD7C:
    addi r29, r29, 0x24
@L_800FFD80:
    cmplwi r30, 0x0
    subi r30, r30, 0x1
    bne @L_800FFCF8
    lwz r0, -23404(r13)
    lwz r4, -23384(r13)
    mulli r0, r0, 0x24
    lwz r3, -23408(r13)
    lwz r31, 0x4(r4)
    lwz r30, -23400(r13)
    add r29, r3, r0
    b @L_800FFE34
@L_800FFDAC:
    lwz r3, 0xc(r29)
    cmpwi r3, 0x1
    bne @L_800FFE30
    lwz r0, 0x10(r29)
    cmplw r0, r31
    bne @L_800FFE30
    cmpwi r3, 0x1
    bne @L_800FFDE0
    lwz r0, 0x1c(r29)
    cmplwi r0, 0x0
    beq @L_800FFDE0
    lhz r3, 0x20(r29)
    bl fn_800F7274
@L_800FFDE0:
    li r0, 0x0
    stw r0, 0x8(r29)
    lwz r3, 0x0(r29)
    cmplwi r3, 0x0
    beq @L_800FFDFC
    lwz r0, 0x4(r29)
    stw r0, 0x4(r3)
@L_800FFDFC:
    lwz r3, 0x4(r29)
    cmplwi r3, 0x0
    beq @L_800FFE10
    lwz r0, 0x0(r29)
    stw r0, 0x0(r3)
@L_800FFE10:
    lwz r0, -23380(r13)
    cmplw r0, r29
    bne @L_800FFE24
    lwz r0, 0x4(r29)
    stw r0, -23380(r13)
@L_800FFE24:
    li r0, 0x0
    stw r0, 0x0(r29)
    stw r0, 0x4(r29)
@L_800FFE30:
    addi r29, r29, 0x24
@L_800FFE34:
    cmplwi r30, 0x0
    subi r30, r30, 0x1
    bne @L_800FFDAC
    lwz r3, -23404(r13)
    lwz r0, -23400(r13)
    lwz r4, -23384(r13)
    add r0, r3, r0
    lwz r3, -23408(r13)
    mulli r0, r0, 0x24
    lwz r31, 0x4(r4)
    lwz r30, -23396(r13)
    add r29, r3, r0
    b @L_800FFEF0
@L_800FFE68:
    lwz r3, 0xc(r29)
    cmpwi r3, 0x1
    bne @L_800FFEEC
    lwz r0, 0x10(r29)
    cmplw r0, r31
    bne @L_800FFEEC
    cmpwi r3, 0x1
    bne @L_800FFE9C
    lwz r0, 0x1c(r29)
    cmplwi r0, 0x0
    beq @L_800FFE9C
    lhz r3, 0x20(r29)
    bl fn_800F7274
@L_800FFE9C:
    li r0, 0x0
    stw r0, 0x8(r29)
    lwz r3, 0x0(r29)
    cmplwi r3, 0x0
    beq @L_800FFEB8
    lwz r0, 0x4(r29)
    stw r0, 0x4(r3)
@L_800FFEB8:
    lwz r3, 0x4(r29)
    cmplwi r3, 0x0
    beq @L_800FFECC
    lwz r0, 0x0(r29)
    stw r0, 0x0(r3)
@L_800FFECC:
    lwz r0, -23380(r13)
    cmplw r0, r29
    bne @L_800FFEE0
    lwz r0, 0x4(r29)
    stw r0, -23380(r13)
@L_800FFEE0:
    li r0, 0x0
    stw r0, 0x0(r29)
    stw r0, 0x4(r29)
@L_800FFEEC:
    addi r29, r29, 0x24
@L_800FFEF0:
    cmplwi r30, 0x0
    subi r30, r30, 0x1
    bne @L_800FFE68
    lwz r4, -23384(r13)
    lwz r3, -32008(r13)
    lwz r29, 0x0(r4)
    bl fn_80112380
    rlwinm. r0, r3, 0, 24, 31
    bne @L_800FFF18
    bl fn_8011274C
@L_800FFF18:
    bl fn_80117C84
    li r3, 0x1
    bl fn_8018DB04
    lwz r3, -23384(r13)
    lwz r3, 0x4(r3)
    bl fn_800F716C
    lwz r3, -23384(r13)
    lwz r3, 0x4(r3)
    bl fn_800F04C4
    bl fn_801024C0
    li r3, 0x0
    bl fn_800D2B90
    mr r3, r29
    bl fn_80115A38
    bl fn_8017B1CC
    mr r3, r29
    bl fn_80115A80
    bl fn_800F915C
    bl fn_80169DF8
    bl fn_80175B94
    bl fn_8016AAAC
    bl fn_800E8EFC
    lwz r3, -23384(r13)
    li r4, 0x0
    stb r4, 0xa(r3)
    lwz r3, -23384(r13)
    stb r4, 0xb(r3)
    lwz r3, -32008(r13)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_800FFFF0
    lwz r3, -23372(r13)
    lwz r5, -32008(r13)
    lwz r4, -23376(r13)
    mtctr r3
    cmplwi r3, 0x0
    beq @L_800FFFC8
@L_800FFFAC:
    lwz r0, 0xc(r4)
    subi r3, r3, 0x1
    cmplw r0, r5
    bne @L_800FFFC0
    b @L_800FFFCC
@L_800FFFC0:
    addi r4, r4, 0x4c
    bdnz @L_800FFFAC
@L_800FFFC8:
    li r4, 0x0
@L_800FFFCC:
    lwz r3, -23384(r13)
    li r0, 0x1
    stw r4, 0x0(r3)
    lwz r4, -32008(r13)
    lwz r3, -23384(r13)
    addi r4, r4, 0x5001
    stw r4, 0x4(r3)
    stw r0, -23368(r13)
    b @L_800FF980
@L_800FFFF0:
    stw r4, -23368(r13)
    b @L_800FF980
@L_800FFFF8:
    bl fn_80112780
    lwz r4, -23384(r13)
    li r0, 0x1
    lis r3, -32704
    li r31, 0x0
    stb r0, 0xa(r4)
    addi r29, r3, 18712
    lwz r3, -23384(r13)
    stb r31, 0xb(r3)
    lwz r3, -23384(r13)
    lwz r30, -23360(r13)
    lwz r3, 0x0(r3)
    lbz r0, 0x0(r3)
    rlwinm r28, r0, 27, 29, 31
    b @L_80100060
@L_80100034:
    lbz r0, 0x0(r29)
    cmplw r0, r28
    bne @L_8010005C
    lwz r12, 0xc(r29)
    mtctr r12
    bctrl
    addi r0, r3, 0x3
    rlwinm r0, r0, 0, 0, 29
    add r31, r0, r31
    addi r31, r31, 0x4
@L_8010005C:
    addi r29, r29, 0x10
@L_80100060:
    cmplwi r30, 0x0
    subi r30, r30, 0x1
    bne @L_80100034
    mr r3, r31
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    mr r31, r3
    bne @L_80100088
    li r31, 0x0
    b @L_80100104
@L_80100088:
    bl fn_800E27B0
    mr. r29, r3
    bne @L_8010009C
    li r31, 0x0
    b @L_80100104
@L_8010009C:
    lis r3, -32704
    lwz r27, -23360(r13)
    addi r25, r3, 18712
    b @L_801000F0
@L_801000AC:
    lbz r0, 0x0(r25)
    cmplw r0, r28
    bne @L_801000EC
    lwz r12, 0xc(r25)
    mtctr r12
    bctrl
    addi r0, r3, 0x3
    addi r30, r29, 0x4
    rlwinm r26, r0, 0, 0, 29
    stw r26, 0x0(r29)
    mr r3, r30
    mr r4, r26
    lwz r12, 0x8(r25)
    mtctr r12
    bctrl
    add r29, r30, r26
@L_801000EC:
    addi r25, r25, 0x10
@L_801000F0:
    cmplwi r27, 0x0
    subi r27, r27, 0x1
    bne @L_801000AC
    mr r3, r31
    bl fn_800E24B0
@L_80100104:
    lwz r3, -23384(r13)
    li r30, 0x1
    sth r31, 0x8(r3)
    lwz r3, -23384(r13)
    lwz r28, -23408(r13)
    lwz r31, 0x4(r3)
    lwz r29, -23404(r13)
    b @L_80100160
@L_80100124:
    lwz r0, 0xc(r28)
    cmpwi r0, 0x0
    bne @L_8010015C
    lwz r0, 0x10(r28)
    cmplw r0, r31
    bne @L_8010015C
    stb r30, 0x15(r28)
    lwz r0, 0xc(r28)
    cmpwi r0, 0x1
    bne @L_8010015C
    lwz r3, 0x1c(r28)
    cmplwi r3, 0x0
    beq @L_8010015C
    bl fn_800F0438
@L_8010015C:
    addi r28, r28, 0x24
@L_80100160:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_80100124
    lwz r0, -23404(r13)
    li r30, 0x1
    lwz r4, -23384(r13)
    mulli r0, r0, 0x24
    lwz r3, -23408(r13)
    lwz r31, 0x4(r4)
    lwz r29, -23400(r13)
    add r28, r3, r0
    b @L_801001CC
@L_80100190:
    lwz r0, 0xc(r28)
    cmpwi r0, 0x0
    bne @L_801001C8
    lwz r0, 0x10(r28)
    cmplw r0, r31
    bne @L_801001C8
    stb r30, 0x15(r28)
    lwz r0, 0xc(r28)
    cmpwi r0, 0x1
    bne @L_801001C8
    lwz r3, 0x1c(r28)
    cmplwi r3, 0x0
    beq @L_801001C8
    bl fn_800F0438
@L_801001C8:
    addi r28, r28, 0x24
@L_801001CC:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_80100190
    lwz r3, -23404(r13)
    li r30, 0x1
    lwz r0, -23400(r13)
    lwz r4, -23384(r13)
    add r0, r3, r0
    lwz r3, -23408(r13)
    mulli r0, r0, 0x24
    lwz r31, 0x4(r4)
    lwz r29, -23396(r13)
    add r28, r3, r0
    b @L_80100240
@L_80100204:
    lwz r0, 0xc(r28)
    cmpwi r0, 0x0
    bne @L_8010023C
    lwz r0, 0x10(r28)
    cmplw r0, r31
    bne @L_8010023C
    stb r30, 0x15(r28)
    lwz r0, 0xc(r28)
    cmpwi r0, 0x1
    bne @L_8010023C
    lwz r3, 0x1c(r28)
    cmplwi r3, 0x0
    beq @L_8010023C
    bl fn_800F0438
@L_8010023C:
    addi r28, r28, 0x24
@L_80100240:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_80100204
    lwz r3, -23384(r13)
    li r30, 0x1
    lwz r28, -23408(r13)
    lwz r31, 0x4(r3)
    lwz r29, -23404(r13)
    b @L_801002A0
@L_80100264:
    lwz r0, 0xc(r28)
    cmpwi r0, 0x1
    bne @L_8010029C
    lwz r0, 0x10(r28)
    cmplw r0, r31
    bne @L_8010029C
    stb r30, 0x15(r28)
    lwz r0, 0xc(r28)
    cmpwi r0, 0x1
    bne @L_8010029C
    lwz r3, 0x1c(r28)
    cmplwi r3, 0x0
    beq @L_8010029C
    bl fn_800F0438
@L_8010029C:
    addi r28, r28, 0x24
@L_801002A0:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_80100264
    lwz r0, -23404(r13)
    li r30, 0x1
    lwz r4, -23384(r13)
    mulli r0, r0, 0x24
    lwz r3, -23408(r13)
    lwz r31, 0x4(r4)
    lwz r29, -23400(r13)
    add r28, r3, r0
    b @L_8010030C
@L_801002D0:
    lwz r0, 0xc(r28)
    cmpwi r0, 0x1
    bne @L_80100308
    lwz r0, 0x10(r28)
    cmplw r0, r31
    bne @L_80100308
    stb r30, 0x15(r28)
    lwz r0, 0xc(r28)
    cmpwi r0, 0x1
    bne @L_80100308
    lwz r3, 0x1c(r28)
    cmplwi r3, 0x0
    beq @L_80100308
    bl fn_800F0438
@L_80100308:
    addi r28, r28, 0x24
@L_8010030C:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_801002D0
    lwz r3, -23404(r13)
    li r30, 0x1
    lwz r0, -23400(r13)
    lwz r4, -23384(r13)
    add r0, r3, r0
    lwz r3, -23408(r13)
    mulli r0, r0, 0x24
    lwz r31, 0x4(r4)
    lwz r29, -23396(r13)
    add r28, r3, r0
    b @L_80100380
@L_80100344:
    lwz r0, 0xc(r28)
    cmpwi r0, 0x1
    bne @L_8010037C
    lwz r0, 0x10(r28)
    cmplw r0, r31
    bne @L_8010037C
    stb r30, 0x15(r28)
    lwz r0, 0xc(r28)
    cmpwi r0, 0x1
    bne @L_8010037C
    lwz r3, 0x1c(r28)
    cmplwi r3, 0x0
    beq @L_8010037C
    bl fn_800F0438
@L_8010037C:
    addi r28, r28, 0x24
@L_80100380:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_80100344
    lwz r3, -23384(r13)
    lwz r3, 0x4(r3)
    bl fn_800F03D4
    lwz r4, -23384(r13)
    lwz r3, -32008(r13)
    lwz r28, 0x0(r4)
    bl fn_80112380
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801003B4
    bl fn_8011274C
@L_801003B4:
    bl fn_80117C84
    li r3, 0x0
    bl fn_8018DB04
    bl fn_801024C0
    li r3, 0x0
    bl fn_800D2B90
    mr r3, r28
    bl fn_80115A38
    bl fn_8017B1CC
    mr r3, r28
    bl fn_80115A80
    bl fn_800F915C
    bl fn_80169DF8
    bl fn_80175B94
    bl fn_8016AAAC
    bl fn_800E8EFC
    lwz r4, -23388(r13)
    li r3, 0x1
    lwz r5, -23420(r13)
    li r0, 0x0
    addi r6, r4, 0x1
    mulli r4, r6, 0x14
    stw r6, -23388(r13)
    add r4, r5, r4
    stw r4, -23384(r13)
    stb r3, 0xa(r4)
    lwz r3, -23384(r13)
    stb r0, 0xb(r3)
    lwz r3, -23372(r13)
    lwz r5, -32008(r13)
    lwz r4, -23376(r13)
    mtctr r3
    cmplwi r3, 0x0
    beq @L_80100458
@L_8010043C:
    lwz r0, 0xc(r4)
    subi r3, r3, 0x1
    cmplw r0, r5
    bne @L_80100450
    b @L_8010045C
@L_80100450:
    addi r4, r4, 0x4c
    bdnz @L_8010043C
@L_80100458:
    li r4, 0x0
@L_8010045C:
    lwz r3, -23384(r13)
    li r0, 0x1
    stw r4, 0x0(r3)
    lwz r4, -32008(r13)
    lwz r3, -23384(r13)
    addi r4, r4, 0x5001
    stw r4, 0x4(r3)
    stw r0, -23368(r13)
    b @L_800FF980
@L_80100480:
    lwz r3, -23384(r13)
    lwz r28, -23408(r13)
    lwz r30, 0x4(r3)
    lwz r29, -23404(r13)
    b @L_8010051C
@L_80100494:
    lwz r3, 0xc(r28)
    cmpwi r3, 0x0
    bne @L_80100518
    lwz r0, 0x10(r28)
    cmplw r0, r30
    bne @L_80100518
    cmpwi r3, 0x1
    bne @L_801004C8
    lwz r0, 0x1c(r28)
    cmplwi r0, 0x0
    beq @L_801004C8
    lhz r3, 0x20(r28)
    bl fn_800F7274
@L_801004C8:
    li r0, 0x0
    stw r0, 0x8(r28)
    lwz r3, 0x0(r28)
    cmplwi r3, 0x0
    beq @L_801004E4
    lwz r0, 0x4(r28)
    stw r0, 0x4(r3)
@L_801004E4:
    lwz r3, 0x4(r28)
    cmplwi r3, 0x0
    beq @L_801004F8
    lwz r0, 0x0(r28)
    stw r0, 0x0(r3)
@L_801004F8:
    lwz r0, -23380(r13)
    cmplw r0, r28
    bne @L_8010050C
    lwz r0, 0x4(r28)
    stw r0, -23380(r13)
@L_8010050C:
    li r0, 0x0
    stw r0, 0x0(r28)
    stw r0, 0x4(r28)
@L_80100518:
    addi r28, r28, 0x24
@L_8010051C:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_80100494
    lwz r0, -23404(r13)
    lwz r4, -23384(r13)
    mulli r0, r0, 0x24
    lwz r3, -23408(r13)
    lwz r30, 0x4(r4)
    lwz r29, -23400(r13)
    add r28, r3, r0
    b @L_801005D0
@L_80100548:
    lwz r3, 0xc(r28)
    cmpwi r3, 0x0
    bne @L_801005CC
    lwz r0, 0x10(r28)
    cmplw r0, r30
    bne @L_801005CC
    cmpwi r3, 0x1
    bne @L_8010057C
    lwz r0, 0x1c(r28)
    cmplwi r0, 0x0
    beq @L_8010057C
    lhz r3, 0x20(r28)
    bl fn_800F7274
@L_8010057C:
    li r0, 0x0
    stw r0, 0x8(r28)
    lwz r3, 0x0(r28)
    cmplwi r3, 0x0
    beq @L_80100598
    lwz r0, 0x4(r28)
    stw r0, 0x4(r3)
@L_80100598:
    lwz r3, 0x4(r28)
    cmplwi r3, 0x0
    beq @L_801005AC
    lwz r0, 0x0(r28)
    stw r0, 0x0(r3)
@L_801005AC:
    lwz r0, -23380(r13)
    cmplw r0, r28
    bne @L_801005C0
    lwz r0, 0x4(r28)
    stw r0, -23380(r13)
@L_801005C0:
    li r0, 0x0
    stw r0, 0x0(r28)
    stw r0, 0x4(r28)
@L_801005CC:
    addi r28, r28, 0x24
@L_801005D0:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_80100548
    lwz r3, -23404(r13)
    lwz r0, -23400(r13)
    lwz r4, -23384(r13)
    add r0, r3, r0
    lwz r3, -23408(r13)
    mulli r0, r0, 0x24
    lwz r30, 0x4(r4)
    lwz r29, -23396(r13)
    add r28, r3, r0
    b @L_8010068C
@L_80100604:
    lwz r3, 0xc(r28)
    cmpwi r3, 0x0
    bne @L_80100688
    lwz r0, 0x10(r28)
    cmplw r0, r30
    bne @L_80100688
    cmpwi r3, 0x1
    bne @L_80100638
    lwz r0, 0x1c(r28)
    cmplwi r0, 0x0
    beq @L_80100638
    lhz r3, 0x20(r28)
    bl fn_800F7274
@L_80100638:
    li r0, 0x0
    stw r0, 0x8(r28)
    lwz r3, 0x0(r28)
    cmplwi r3, 0x0
    beq @L_80100654
    lwz r0, 0x4(r28)
    stw r0, 0x4(r3)
@L_80100654:
    lwz r3, 0x4(r28)
    cmplwi r3, 0x0
    beq @L_80100668
    lwz r0, 0x0(r28)
    stw r0, 0x0(r3)
@L_80100668:
    lwz r0, -23380(r13)
    cmplw r0, r28
    bne @L_8010067C
    lwz r0, 0x4(r28)
    stw r0, -23380(r13)
@L_8010067C:
    li r0, 0x0
    stw r0, 0x0(r28)
    stw r0, 0x4(r28)
@L_80100688:
    addi r28, r28, 0x24
@L_8010068C:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_80100604
    lwz r3, -23384(r13)
    lwz r28, -23408(r13)
    lwz r30, 0x4(r3)
    lwz r29, -23404(r13)
    b @L_80100734
@L_801006AC:
    lwz r3, 0xc(r28)
    cmpwi r3, 0x1
    bne @L_80100730
    lwz r0, 0x10(r28)
    cmplw r0, r30
    bne @L_80100730
    cmpwi r3, 0x1
    bne @L_801006E0
    lwz r0, 0x1c(r28)
    cmplwi r0, 0x0
    beq @L_801006E0
    lhz r3, 0x20(r28)
    bl fn_800F7274
@L_801006E0:
    li r0, 0x0
    stw r0, 0x8(r28)
    lwz r3, 0x0(r28)
    cmplwi r3, 0x0
    beq @L_801006FC
    lwz r0, 0x4(r28)
    stw r0, 0x4(r3)
@L_801006FC:
    lwz r3, 0x4(r28)
    cmplwi r3, 0x0
    beq @L_80100710
    lwz r0, 0x0(r28)
    stw r0, 0x0(r3)
@L_80100710:
    lwz r0, -23380(r13)
    cmplw r0, r28
    bne @L_80100724
    lwz r0, 0x4(r28)
    stw r0, -23380(r13)
@L_80100724:
    li r0, 0x0
    stw r0, 0x0(r28)
    stw r0, 0x4(r28)
@L_80100730:
    addi r28, r28, 0x24
@L_80100734:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_801006AC
    lwz r0, -23404(r13)
    lwz r4, -23384(r13)
    mulli r0, r0, 0x24
    lwz r3, -23408(r13)
    lwz r30, 0x4(r4)
    lwz r29, -23400(r13)
    add r28, r3, r0
    b @L_801007E8
@L_80100760:
    lwz r3, 0xc(r28)
    cmpwi r3, 0x1
    bne @L_801007E4
    lwz r0, 0x10(r28)
    cmplw r0, r30
    bne @L_801007E4
    cmpwi r3, 0x1
    bne @L_80100794
    lwz r0, 0x1c(r28)
    cmplwi r0, 0x0
    beq @L_80100794
    lhz r3, 0x20(r28)
    bl fn_800F7274
@L_80100794:
    li r0, 0x0
    stw r0, 0x8(r28)
    lwz r3, 0x0(r28)
    cmplwi r3, 0x0
    beq @L_801007B0
    lwz r0, 0x4(r28)
    stw r0, 0x4(r3)
@L_801007B0:
    lwz r3, 0x4(r28)
    cmplwi r3, 0x0
    beq @L_801007C4
    lwz r0, 0x0(r28)
    stw r0, 0x0(r3)
@L_801007C4:
    lwz r0, -23380(r13)
    cmplw r0, r28
    bne @L_801007D8
    lwz r0, 0x4(r28)
    stw r0, -23380(r13)
@L_801007D8:
    li r0, 0x0
    stw r0, 0x0(r28)
    stw r0, 0x4(r28)
@L_801007E4:
    addi r28, r28, 0x24
@L_801007E8:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_80100760
    lwz r3, -23404(r13)
    lwz r0, -23400(r13)
    lwz r4, -23384(r13)
    add r0, r3, r0
    lwz r3, -23408(r13)
    mulli r0, r0, 0x24
    lwz r30, 0x4(r4)
    lwz r29, -23396(r13)
    add r28, r3, r0
    b @L_801008A4
@L_8010081C:
    lwz r3, 0xc(r28)
    cmpwi r3, 0x1
    bne @L_801008A0
    lwz r0, 0x10(r28)
    cmplw r0, r30
    bne @L_801008A0
    cmpwi r3, 0x1
    bne @L_80100850
    lwz r0, 0x1c(r28)
    cmplwi r0, 0x0
    beq @L_80100850
    lhz r3, 0x20(r28)
    bl fn_800F7274
@L_80100850:
    li r0, 0x0
    stw r0, 0x8(r28)
    lwz r3, 0x0(r28)
    cmplwi r3, 0x0
    beq @L_8010086C
    lwz r0, 0x4(r28)
    stw r0, 0x4(r3)
@L_8010086C:
    lwz r3, 0x4(r28)
    cmplwi r3, 0x0
    beq @L_80100880
    lwz r0, 0x0(r28)
    stw r0, 0x0(r3)
@L_80100880:
    lwz r0, -23380(r13)
    cmplw r0, r28
    bne @L_80100894
    lwz r0, 0x4(r28)
    stw r0, -23380(r13)
@L_80100894:
    li r0, 0x0
    stw r0, 0x0(r28)
    stw r0, 0x4(r28)
@L_801008A0:
    addi r28, r28, 0x24
@L_801008A4:
    cmplwi r29, 0x0
    subi r29, r29, 0x1
    bne @L_8010081C
    lwz r4, -23384(r13)
    lwz r3, -32008(r13)
    lwz r28, 0x0(r4)
    bl fn_80112380
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801008CC
    bl fn_8011274C
@L_801008CC:
    bl fn_80117C84
    li r3, 0x1
    bl fn_8018DB04
    lwz r3, -23384(r13)
    lwz r3, 0x4(r3)
    bl fn_800F716C
    lwz r3, -23384(r13)
    lwz r3, 0x4(r3)
    bl fn_800F04C4
    bl fn_801024C0
    li r3, 0x0
    bl fn_800D2B90
    mr r3, r28
    bl fn_80115A38
    bl fn_8017B1CC
    mr r3, r28
    bl fn_80115A80
    bl fn_800F915C
    bl fn_8010D038
    bl fn_80169DF8
    bl fn_80175B94
    bl fn_8016AAAC
    bl fn_800E8EFC
    lwz r4, -23388(r13)
    li r3, 0x0
    lwz r6, -23420(r13)
    li r0, 0x1
    subi r7, r4, 0x1
    li r4, 0x1
    mulli r5, r7, 0x14
    stw r7, -23388(r13)
    add r5, r6, r5
    stw r5, -23384(r13)
    stb r3, 0xa(r5)
    lwz r3, -23384(r13)
    stb r0, 0xb(r3)
    lwz r3, -23384(r13)
    lwz r28, 0x0(r3)
    mr r3, r28
    bl fn_801123D4
    mr r3, r28
    bl fn_80115A38
    bl fn_8017B3E4
    lis r3, -32729
    addi r31, r3, 6164
@L_80100980:
    mr r3, r28
    bl fn_80115A38
    bl fn_8017B2CC
    mr. r30, r3
    bge @L_801009A0
    mr r3, r31
    crxor 6, 6, 6
    bl fn_800DD970
@L_801009A0:
    cmpwi r30, 0x0
    beq @L_801009B0
    bl fn_800F0308
    b @L_80100980
@L_801009B0:
    mr r3, r28
    bl fn_80115A80
    bl fn_800F0308
    bl fn_800F0308
    lwz r4, -23384(r13)
    lwz r3, 0x0(r4)
    lhz r27, 0x8(r4)
    lbz r0, 0x0(r3)
    mr r3, r27
    rlwinm r26, r0, 27, 29, 31
    bl fn_800E27B0
    cmplwi r3, 0x0
    beq @L_80100A40
    lis r4, -32704
    lwz r28, -23360(r13)
    addi r31, r4, 18712
    b @L_80100A24
@L_801009F4:
    lbz r0, 0x0(r31)
    cmplw r0, r26
    bne @L_80100A20
    lwz r29, 0x0(r3)
    addi r30, r3, 0x4
    lwz r12, 0x4(r31)
    mr r3, r30
    mr r4, r29
    mtctr r12
    bctrl
    add r3, r30, r29
@L_80100A20:
    addi r31, r31, 0x10
@L_80100A24:
    cmplwi r28, 0x0
    subi r28, r28, 0x1
    bne @L_801009F4
    mr r3, r27
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
@L_80100A40:
    lwz r3, -23384(r13)
    li r31, 0x0
    lwz r26, -23408(r13)
    lwz r28, 0x4(r3)
    lwz r27, -23404(r13)
    b @L_80100A94
@L_80100A58:
    lwz r0, 0xc(r26)
    cmpwi r0, 0x0
    bne @L_80100A90
    lwz r0, 0x10(r26)
    cmplw r0, r28
    bne @L_80100A90
    stb r31, 0x15(r26)
    lwz r0, 0xc(r26)
    cmpwi r0, 0x1
    bne @L_80100A90
    lwz r3, 0x1c(r26)
    cmplwi r3, 0x0
    beq @L_80100A90
    bl fn_800F0424
@L_80100A90:
    addi r26, r26, 0x24
@L_80100A94:
    cmplwi r27, 0x0
    subi r27, r27, 0x1
    bne @L_80100A58
    lwz r3, -23384(r13)
    li r31, 0x0
    lwz r26, -23408(r13)
    lwz r28, 0x4(r3)
    lwz r27, -23404(r13)
    b @L_80100AF4
@L_80100AB8:
    lwz r0, 0xc(r26)
    cmpwi r0, 0x1
    bne @L_80100AF0
    lwz r0, 0x10(r26)
    cmplw r0, r28
    bne @L_80100AF0
    stb r31, 0x15(r26)
    lwz r0, 0xc(r26)
    cmpwi r0, 0x1
    bne @L_80100AF0
    lwz r3, 0x1c(r26)
    cmplwi r3, 0x0
    beq @L_80100AF0
    bl fn_800F0424
@L_80100AF0:
    addi r26, r26, 0x24
@L_80100AF4:
    cmplwi r27, 0x0
    subi r27, r27, 0x1
    bne @L_80100AB8
    lwz r3, -23384(r13)
    lwz r3, 0x4(r3)
    bl fn_800F0384
    li r0, 0x2
    lwz r3, -23384(r13)
    stw r0, -23368(r13)
    li r0, 0x1
    stw r0, 0x10(r3)
    b @L_800FF980
